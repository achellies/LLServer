LLServer
========

A nosql database base on libevent,leveldb.support memcached protocal.

LLServer 1.0 is a high-performance, distributed string store system.it support http , memcached protocal, base on libevent, leveldb project. LLServer�Ǳ��˻���libevent��leveldb��������Դ��������������������ݴ洢���������������libevent��Ч����ӿ�ʵ�ֶ�leveldb�ķ��ʷ�װ�� 

��Ŀ��ַ��http://code.google.com/p/llserver/ �� 
ʹ�û�����Linux �� 
���ߣ������ �� 
Ŀǰ�����汾��1.0 

�� ֧��httpЭ���memcachedЭ�顣Ҳ���ǿ���ͨ������������е�memcached�ͻ������������ݵ�CURD������ 

����򵥽���һ����ΰ�װʹ��LLServer�� 

LLServer ���밲װ�� 

1.��װlibevent2.0 
================================
ulimit -SHn 65535 wget http://monkey.org/~provos/libevent-2.0.12-stable.tar.gz 
tar zxvf libevent-2.0.12-stable.tar.gz 
cd libevent-2.0.12-stable/ 
./configure --prefix=/usr 
make && make install 
cd ../ 

2.ͨ��svn:�ͻ�������leveldb������leveldb�ļ���
=============================================
���ӣ�http://leveldb.googlecode.com/svn/trunk/ ����ʱ���������ð��ĵ�ַ���أ� 
wget http://llserver.googlecode.com/files/leveldb.tar.gz 
tar zxvf leveldb.tar.gz 

֮����밲װ cd leveldb/ 
make -f Makefile 
cp libleveldb.a /usr/local/lib/ 
cp -rf include/ /usr/local/include/ 
cd ../ 

3.LLServer���ص�ַ��
==============================================
wget http://llserver.googlecode.com/files/llserver-1.0.tar.gz 
tar zxvf llserver-1.0.tar.gz 
cd llserver/ 
make -f Makefile 
make install 
cd ../ 

4��LLServer ʹ���ĵ�\
==============================================
[root@~]# llserver -h 
-l 
<ip_addr>������IP��ַ��Ĭ��ֵΪ 0.0.0.0 -p 
<num>������TCP�˿ڣ�Ĭ��ֵ��11211�� -x 
<path>���ݿ�Ŀ¼��Ŀ¼�����ڻ��Զ����������磺/llserver/data�� -c ���ݻ�����е�λ��Ĭ��Ϊ100m -t 
<second>HTTP����ĳ�ʱʱ�� -s 1:httpЭ�� other:memcachedЭ�� -d ���ػ��������� -h ��ʾ���� �������ݲμ��� 

http://www.cnblogs.com/daizhj/archive/2011/08/23/2150422.html 
http://www.cnblogs.com/daizhj/ 
http://weibo.com/daizhj 