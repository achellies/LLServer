#include <string.h>
#include <ctype.h>
#include <malloc.h> /* malloc()�� */
#include <limits.h> /* INT_MAX�� */
#include <stdio.h> /* EOF(=^Z��F6),NULL */
#include <stdlib.h> /* atoi() */
#include <stdio.h> /* eof() */
#include <math.h> /* floor(),ceil(),abs() */
#ifndef WIN32
#include <errno.h>
#else
#include <process.h> /* exit() */
#endif
#include "hstring.h"

hstring hs_init(char *data)
{
	hstring str;
	/* ��ʼ��(�����մ�)�ַ���T����� */
	if(data != NULL)
	{
	  str.length = strlen(data);
	  str.ch = data;
	}
	else
    {
	  str.length = 0;
	  str.ch = NULL;
	}
	return str;
}


void hs_init(hstring *pstr)
{ /* ��ʼ��(�����մ�)�ַ���T����� */
	(*pstr).length = 0;
	(*pstr).ch = NULL;
}


status hs_assign(hstring *pstr, char *chars)
{ /* ����һ����ֵ���ڴ�����chars�Ĵ�T */
	int i = strlen(chars); /* ��chars�ĳ���i */
	if((*pstr).ch)
		free((*pstr).ch); /* �ͷ�Tԭ�пռ� */

	if(!i) { /* chars�ĳ���Ϊ0 */
		(*pstr).ch = NULL;
		(*pstr).length = 0;
	}
	else { /* chars�ĳ��Ȳ�Ϊ0 */
		(*pstr).ch = (char*)malloc(i*sizeof(char)); /* ���䴮�ռ� */
		if(!(*pstr).ch) /* ���䴮�ռ�ʧ�� */
			exit(OVERFLOW);
		for(int j = 0 ;j < i ;j++) /* ������ */
			(*pstr).ch[j] = chars[j];
		(*pstr).length = i;
	}
	return OK;
}

status hs_copy(hstring *pstr,hstring s)
{ /* ��ʼ����:��S���ڡ��������: �ɴ�S���Ƶô�T */
	if((*pstr).ch)
		free((*pstr).ch); /* �ͷ�Tԭ�пռ� */
	(*pstr).ch=(char*)malloc(s.length*sizeof(char)); /* ���䴮�ռ� */
	if(!(*pstr).ch) /* ���䴮�ռ�ʧ�� */
		exit(OVERFLOW);
	for(int i = 0 ;i < s.length ;i++) /* ������ */
		(*pstr).ch[i] = s.ch[i];
	(*pstr).length = s.length;
	return OK;
}

status hs_empty(hstring s)
{ /* ��ʼ����: ��s���ڡ��������: ��sΪ�մ�,�򷵻�TRUE,���򷵻�FALSE */
	if(s.length==0&&s.ch==NULL)
		return TRUE;
	else
		return FALSE;
}

int hs_compare(hstring s1,hstring s2)
{ /* ��s1>s2,�򷵻�ֵ>0;��s1=s2,�򷵻�ֵ=0;��s1<s2,�򷵻�ֵ<0 */
	int i;
	for(i=0;i<s1.length&&i<s2.length;++i)
		if(s1.ch[i]!=s2.ch[i])
			return s1.ch[i]-s2.ch[i];
	return s1.length-s2.length;
}

int hs_length(hstring s)
{ /* ����S��Ԫ�ظ���,��Ϊ���ĳ��� */
	return s.length;
}

status hs_clear(hstring *pstr)
{ /* ��S��Ϊ�մ�*/
	if((*pstr).ch)
	{
		free((*pstr).ch);
		(*pstr).ch=NULL;
	}
	(*pstr).length=0;
	return OK;
}

status hs_concat(hstring *pstr,hstring s1,hstring s2)
{ /* ��T������s1��s2���Ӷ��ɵ��´� */
	int i;
	if((*pstr).ch)
		free((*pstr).ch); /* �ͷžɿռ� */
	(*pstr).length=s1.length+s2.length;
	(*pstr).ch=(char *)malloc((*pstr).length*sizeof(char));
	if(!(*pstr).ch)
		exit(OVERFLOW);
	for(i=0;i<s1.length;i++)
		(*pstr).ch[i]=s1.ch[i];
	for(i=0;i<s2.length;i++)
		(*pstr).ch[s1.length+i]=s2.ch[i];
	return OK;
}

status hs_substring(hstring *pstr, hstring s,int pos,int len)
{ /* ��sub���ش�s�ĵ�pos���ַ��𳤶�Ϊlen���Ӵ���*/
	/* ����,1<=pos<=hs_length(s)��<=len<=hs_strlength(s)-pos+1 */
	if(pos<1||pos>s.length||len<0||len>s.length-pos+1)
		return ERROR;
	if((*pstr).ch)
		free((*pstr).ch); /* �ͷžɿռ�*/
	if(!len) /* ���Ӵ�*/
	{
		(*pstr).ch = NULL;
		(*pstr).length = 0;
	}
	else
	{ /* �����Ӵ�*/
		(*pstr).ch = (char*)malloc((len+1)*sizeof(char));
		if(!(*pstr).ch)
			exit(OVERFLOW);
		for(int i = 0; i <= len-1; i++)
			(*pstr).ch[i] = s.ch[pos-1+i];
		(*pstr).length=len;
		(*pstr).ch[len]='\0';
	}
	return OK;
}


int hs_find(hstring s1,hstring s2,int pos) 
{   /* s2Ϊ�ǿմ���������s1�е�pos���ַ�֮�������s2��ȵ��Ӵ�, */
	/* �򷵻ص�һ���������Ӵ���s1�е�λ��,���򷵻�0 */
	int n,m,i;
	hstring sub;
	hs_init(&sub);
	if(pos>0){
		n=hs_length(s1);
		m=hs_length(s2);
		i=pos;
		while(i <= (n-m+1)){
			hs_substring(&sub, s1, i, m);
			if(hs_compare(sub, s2)!=0)
				++i;
			else
				return i;
		}
	}
	return 0;
}

int hs_findfirst(hstring s1, hstring s2)
{
   return hs_find(s1, s2, 1);
}


int hs_findlast(hstring s1, hstring s2)
{
   if(s1.length == 0 || s1.length < s2.length)
	   return FALSE;

   int lastfindpos = hs_find(s1, s2, 1);
   if(lastfindpos == 0)
	   return 0;
   //���ж����ַ��ɹ�����
   int toend = s1.length - lastfindpos - s2.length;
   int findpos = 0;
   
   while(toend >= s2.length){
	   findpos = hs_find(s1, s2, lastfindpos + s2.length);
	   if(findpos)
	   {
          lastfindpos = findpos;
		  toend = s1.length - findpos - s2.length;
	   }
	   else
	      break; 
   }
   return lastfindpos;
}

int hs_findcount(hstring s1, hstring s2)
{
   if(s1.length == 0 || s1.length < s2.length)
	   return FALSE;

   int lastfindpos = hs_find(s1, s2, 1);
   if(lastfindpos == 0)
	   return 0;
   //���ж����ַ��ɹ�����
   int toend = s1.length - lastfindpos - s2.length;
   int findpos = 0;
   int count = 1;//֮ǰlastfindpos!=0��ʾ�Ѳ鵽һ�� 
   while(toend >= s2.length){
	   findpos = hs_find(s1, s2, lastfindpos + s2.length);
	   if(findpos)
	   {
          lastfindpos = findpos;
		  toend = s1.length - findpos - s2.length;
		  count++;
	   }
	   else
	      break; 
   }
   return count;
}

status hs_insert(hstring *pstr, int pos, hstring s) 
{ /* 1��pos��hs_length(pstr)+1���ڴ�pstr�ĵ�pos���ַ�֮ǰ���봮s */
	int i;
	if(pos<1||pos>(*pstr).length+1) /* pos���Ϸ� */
		return ERROR;
	if(s.length) {/* s�ǿ�,�����·���ռ�,����s */
		(*pstr).ch=(char*)realloc((*pstr).ch,((*pstr).length+s.length)*sizeof(char));
		if(!(*pstr).ch)
			exit(OVERFLOW);
		for(i = (*pstr).length-1; i>=pos-1; --i) /* Ϊ����T���ڳ�λ�� */
			(*pstr).ch[i+s.length] = (*pstr).ch[i];
		for(i = 0; i<s.length; i++)
			(*pstr).ch[pos-1+i] = s.ch[i]; /* ����s */
		(*pstr).length += s.length;
	}
	return OK;
}

status hs_delete(hstring *pstr,int pos,int len)
{ /* �Ӵ�S��ɾ����pos���ַ��𳤶�Ϊlen���Ӵ� */
	if((*pstr).length<pos+len-1)
		exit(ERROR);
	for(int i = pos-1; i<=(*pstr).length-len; i++)
		(*pstr).ch[i] = (*pstr).ch[i+len];
	(*pstr).length -= len;
	(*pstr).ch = (char*)realloc((*pstr).ch, (*pstr).length * sizeof(char));
	return OK;
}

status hs_replace(hstring *pstr,hstring s1,hstring s2)
{   /* ��ʼ����: ��pstr, s1��s2����,s1�Ƿǿմ����˺����봮�Ĵ洢�ṹ�޹أ� */
	/* �������: ��s1�滻����pstr�г��ֵ�������s1��ȵĲ��ص����Ӵ� */
	int i=1; /* �Ӵ�S�ĵ�һ���ַ�����Ҵ�s1 */
	if(hs_empty(s1)) /* s1�ǿմ� */
		return ERROR;
	do
	{
		i=hs_find(*pstr,s1,i); /* ���iΪ����һ��i֮���ҵ����Ӵ�T��λ�� */
		if(i) /* ��pstr�д��ڴ�s1 */
		{
			hs_delete(pstr, i, hs_length(s1)); /* ɾ���ô�s1 */
			hs_insert(pstr, i, s2); /* ��ԭ��s1��λ�ò��봮s2 */
			i += hs_length(s2); /* �ڲ���Ĵ�s2����������Ҵ�s1 */
		}
	} while(i);
	return OK;
}


void hs_print(hstring s)
{ /* ���s�ַ�������� */
	for(int i = 0 ;i < s.length ;i++)
		printf("%c",s.ch[i]);
	printf("\n");
}


status hs_startwith(hstring s1,hstring s2)
{   /* s2Ϊ�ǿմ���������s1�е�pos���ַ�֮�������s2��ȵ��Ӵ�, */	
	if(s1.length == 0 || s1.length < s2.length)
	   return FALSE;
    int i;
	for(i = 0; i < s2.length ; i++){
		if(s1.ch[i] != s2.ch[i])
		   return FALSE;
	}
	return TRUE;
}

status hs_endwith(hstring s1,hstring s2)
{   /* s2Ϊ�ǿմ���������s1�е�pos���ַ�֮�������s2��ȵ��Ӵ�, */	
	if(s1.length == 0 || s1.length < s2.length)
	   return FALSE;
    int i;
	for(i = 1; i <= s2.length ; i++){
		if(s1.ch[s1.length- i] != s2.ch[s2.length - i])
		   return FALSE;
	}
	return TRUE;
}


/*
int main(int argc, char *argv[])
{
	int i;
	char c,*p="God bye!",*q="God luck!";
	hstring t,s,r;
	hs_init(&t); //hstring���ͱ����ʼ��
	hs_init(&s);
	hs_init(&r);
	hs_assign(&t,p);
	printf("��tΪ: ");
	hs_print(t);
	printf("����Ϊ%d ���շ�%d(1:�� 0:��)\n",hs_length(t),hs_empty(t));
	hs_assign(&s,q);
	printf("��sΪ: ");
	hs_print(s);
	i=hs_compare(s,t);
	if(i<0)
	  c='<';
	else if(i==0)
	  c='=';
	else
	  c='>';
	printf("��s%c��t\n",c);
	hs_concat(&r,t,s);
	printf("��t���Ӵ�s�����Ĵ�rΪ: ");
	hs_print(r);
	hs_assign(&s,"oo");
	printf("��sΪ: ");
	hs_print(s);
	hs_assign(&t,"o");
	printf("��tΪ: ");
	hs_print(t);
	hs_replace(&r,t,s);
	printf("�Ѵ�r�кʹ�t��ͬ���Ӵ��ô�s����󣬴�rΪ:\n");
	hs_print(r);
	hs_clear(&s);
	printf("��s��պ󣬴���Ϊ%d �շ�%d(1:�� 0:��)\n",hs_length(s),hs_empty(s));
	hs_substring(&s,r,6,4);
	printf("��sΪ�Ӵ�r�ĵ�6���ַ����4���ַ�������Ϊ%d ��sΪ: ",s.length);
	hs_print(s);
	hs_copy(&t,r);
	printf("���ƴ�tΪ��r,��tΪ: ");
	hs_print(t);
	hs_insert(&t,6,s);
	printf("�ڴ�t�ĵ�6���ַ�ǰ���봮s�󣬴�tΪ: ");
	hs_print(t);
	hs_delete(&t, 1, 5);
	printf("�Ӵ�t�ĵ�1���ַ���ɾ��5���ַ���,��tΪ: ");
	hs_print(t);
	printf("%d�ǴӴ�t�ĵ�1���ַ��𣬺ʹ�s��ͬ�ĵ�1���Ӵ���λ��\n",hs_find(t,s,1));
	printf("%d�ǴӴ�t�ĵ�2���ַ��𣬺ʹ�s��ͬ�ĵ�1���Ӵ���λ��\n",hs_find(t,s,2));

	hs_assign(&t,"hello world!");
    hs_assign(&s,"hello");  
	printf("t�Ƿ���s��ͷ:%d\n",hs_startwith(t, s));

	hs_assign(&s,"world!");  
	printf("t�Ƿ���s����:%d\n",hs_endwith(t, s));

    hs_assign(&t,"wwww www ww www wwwwwww");
    hs_assign(&s,"ww");  
	printf("t�����һ�γ���s��λ��:%d\n",hs_findlast(t, s));
	printf("t�г���s�Ĵ���:%d\n",hs_findcount(t, s));

    return 0;
}
*/
