#include <sys/types.h>
#include <sys/time.h>
#include <sys/queue.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <iostream>

#include <err.h>
#include <event.h>
#include <evhttp.h>

#include "leveldb/db.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"
#include "leveldb/cache.h"

#include "utils.h"
#include "llserver.h"

/*ȫ�ֱ���*/
char *pidfile; /* PID�ļ� */
int servicetype; /* ͬ���������ݵ����̵ļ��ʱ�� */

/* �ӽ����źŴ��� */
static void kill_signal_worker(const int sig) {
    delete lldb;
    //delete options.cache;
    exit(0);
}

/* �������źŴ��� */
static void kill_signal_master(const int sig) {
    /* ɾ��PID�ļ� */
    remove(pidfile);
    /* �������鷢��SIGTERM�źţ������ӽ��� */
    kill(0, SIGTERM);	
    exit(0);
}

typedef void(*ptr_event)(const int sig);

void kill_singal_register(ptr_event p)
{
	/* ����Broken Pipe�ź� */
	signal(SIGPIPE, SIG_IGN);
	/* ����kill�ź� */
	signal(SIGINT, p);
	signal(SIGKILL, p);
	signal(SIGQUIT, p);
	signal(SIGTERM, p);
	signal(SIGHUP, p);	
}


static void show_help(void)
{
	char *b = "--------------------------------------------------------------------------------------------------\n"
		  "Libevent Leveldb Service - llserver v" VERSION " (8 20, 2011)\n\n"
		  "Author: Zhenjun Dai (http://daizhj.cnblogs.com), E-mail: daizhj617595@126.com\n"
		  "This is free software, and you are welcome to modify and redistribute it under the New BSD License\n"
		  "\n"
		   "-l <ip_addr>  interface to listen on, default is 0.0.0.0\n"
		   "-p <num>      TCP port number to listen on (default: 11211)\n"
		   "-x <path>     database directory (example: /opt/llserver/data)\n"
		   "-t <second>   keep-alive timeout for an http request (default: 60)\n"
		   "-s <service type>  1: httpserver  other:mc-server \n"
		   "-c <num>      the maximum cached data nodes to be cached (default: 100mb)\n"
		   "-i <file>     save PID in <file> (default: /tmp/llserver.pid)\n"
		   "-d            run as a daemon\n"
		   "-h            print this help and exit\n\n"
		   "Use command \"killall llserver\", \"pkill llserver\" to stop llserver.\n"
		   "\n"
		   "Please visit \"http://code.google.com/p/llserver\" for more help information.\n\n"
		   "--------------------------------------------------------------------------------------------------\n"
		   "\n";
	fprintf(stderr, b, strlen(b));
}

/* defaults */
struct settings settings;

static void settings_init(int port);

static void settings_init(int port) {
    settings.port = port;
    settings.udpport = 0;
    settings.interf.s_addr = htonl(INADDR_ANY);
    settings.maxbytes = 67108864;     /* default is 64MB: (64 * 1024 * 1024) */
    settings.maxconns = 1024;         /* to limit connections-related memory to about 5MB */
    settings.verbose = 0;
    settings.evict_to_free = 1;       /* push old items out of cache when memory runs out */
    settings.socketpath = NULL;       /* by default, not using a unix socket */
    settings.managed = false;
    settings.factor = 1.25;
    settings.chunk_size = 48;         /* space for a modest key and value */
    settings.num_threads = 4;
    settings.prefix_delimiter = ':';
    settings.detail_enabled = 0;
}

char *llserver_settings_dataname = (char*) malloc(sizeof(char)*1024);

int llserver_settings_cache = 100;

int httpserver_init(char *listen, int port, int timeout)
{	
	/* �������� */
    struct evhttp *httpd;
    event_init();
    httpd = evhttp_start(listen, port);
	if (httpd == NULL) {
		fprintf(stderr, "Error: Unable to listen on %s:%d\n\n", listen, port);
		kill(0, SIGTERM);
		exit(1);		
	}
	evhttp_set_timeout(httpd, timeout);
    /* Set a callback for all other requests. */
    evhttp_set_gencb(httpd, llserver_http_handler, NULL);
    event_dispatch();
    /* Not reached in this code as it is now. */
    evhttp_free(httpd);
	return 0;
}


int main(int argc, char *argv[], char *envp[])
{
	int c;
	/* Ĭ�ϲ������� */
	char *listen = "0.0.0.0";
	int port = 11211;
	bool daemon = false;
	int timeout = 60; /* ��λ���� */
    char *datapath;
	servicetype = 0; /* 1��httpserver  ����Ϊsocket���� */
	pidfile = "/tmp/llserver.pid";

	/* �����в�������ʱ�洢���棬���ڽ��������� */
	int llserver_prename_num = 1;
	char path_file[1024] = { 0 }; // path_file Ϊ llserver ����ľ���·��
	struct evbuffer *llserver_prename_buf; /* ԭ�����в��� */
    llserver_prename_buf = evbuffer_new();
	readlink("/proc/self/exe", path_file, sizeof(path_file));
	evbuffer_add_printf(llserver_prename_buf, "%s", path_file);
	for (llserver_prename_num = 1; llserver_prename_num < argc; llserver_prename_num++) {
		evbuffer_add_printf(llserver_prename_buf, " %s", argv[llserver_prename_num]);
	}
	
    /* process arguments */
    while ((c = getopt(argc, argv, "l:p:x:t:s:c:m:i:a:dh")) != -1) {
        switch (c) {
        case 'l':
            listen = strdup(optarg);
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'x':
            datapath = strdup(optarg); /* llserver���ݿ��ļ����·�� */
			if (access(datapath, W_OK) != 0) { /* ���Ŀ¼����д */
				if (access(datapath, R_OK) == 0) /* ���Ŀ¼�ɶ� */
					chmod(datapath, S_IWOTH); /* ���������û��߿�д��Ȩ�� */
				else  /* ��������ڸ�Ŀ¼���򴴽� */
					create_multilayer_dir(datapath);
								
				if (access(datapath, W_OK) != 0) /* ���Ŀ¼����д */
					fprintf(stderr, "llserver database directory not writable\n");
			}
            break;
        case 't':
            timeout = atoi(optarg);
            break;
        case 's':
            servicetype = atoi(optarg);
            break;
        case 'c':
			llserver_settings_cache = atoi(optarg)>0 ? atoi(optarg) : 100;
			break;
        case 'i':
            pidfile = strdup(optarg);
            break;
        case 'd':
            daemon = true;
            break;
		case 'h':
        default:
            show_help();
            return 1;
        }
    }
	
	/* �ж��Ƿ���˱������ -x */
	if (datapath == NULL) {
		show_help();
		fprintf(stderr, "Attention: Please use the indispensable argument: -x <path>\n\n");		
		exit(1);
	}

    sprintf(llserver_settings_dataname, "%s/llserver.db", datapath);
        	
	/* �жϱ��Ƿ��ܴ� */
	if(!opendb().ok())
	{
  		show_help();
		fprintf(stderr, "Attention: open db failed.\n\n");		
		exit(1);
	}	

	/* �������-d���������ػ��������� */
	if (daemon == true) {
        pid_t pid = fork();/* Fork off the parent process */       
        if (pid < 0) 
            exit(EXIT_FAILURE);        
        /* If we got a good PID, then we can exit the parent process. */
        if (pid > 0) 
            exit(EXIT_SUCCESS);
    }

	/* �����̺�д��PID�ļ� */
	FILE *fp_pidfile = fopen(pidfile, "w");
	fprintf(fp_pidfile, "%d\n", getpid());
	fclose(fp_pidfile);

    /* ����llserver�ӽ��̣��������̣� */
    pid_t llserver_worker_pid_wait;
    pid_t llserver_worker_pid = fork();
    /* �����������ʧ�ܣ����˳����� */
    if (llserver_worker_pid < 0)
    {
        fprintf(stderr, "Error: %s:%d\n", __FILE__, __LINE__);
	    exit(EXIT_FAILURE);
    }
    /* llserver���������� */
    if (llserver_worker_pid > 0)
    {		
		/* ����kill�ź� */		
		kill_singal_register(&kill_signal_master);
		/* ����δ����ź� */
		//signal(SIGSEGV, kill_signal_master);

        /* ����ӽ�����ֹ�������������µ��ӽ��� */
        while (1)
        {
    	    llserver_worker_pid_wait = wait(NULL);
            if (llserver_worker_pid_wait < 0)
                continue;

	        usleep(100000);
            llserver_worker_pid = fork();
            if (llserver_worker_pid == 0)
                break;
	    }
    }	
	
	/* ����kill�ź� */	
	kill_singal_register(&kill_signal_worker);

	if(servicetype == 1)
	{
		evbuffer_free(llserver_prename_buf);
		httpserver_init(listen, port, timeout);
	}
	else{
        settings_init(port);
        //����memcached����
        start_mc_server();  
	}
    return 0;
}


