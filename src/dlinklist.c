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
#include "dlinklist.h"

status init_dlist(dlinklist *L)
{ 
	/* �����յ�˫��ѭ������L */
	*L=(dlinklist)malloc(sizeof(dnode));
	if(*L)
	{
	   (*L)->next=(*L)->prior=*L;
	   return OK;
	}
	else
	  return OVERFLOW;
}

status destroy_dlist(dlinklist *L)
{
	/* �������������˫��ѭ������L */
    dlinklist q,p=(*L)->next; /* pָ���һ����� */
	while(p!=*L) /* pû����ͷ */
	{
		q=p->next;
		free(p);
		p=q;
	}
	free(*L);
	*L=NULL;
	return OK;
}

status clear_dlist(dlinklist L) /* ���ı�L */
{
	/* ��ʼ������L�Ѵ��ڡ������������L����Ϊ�ձ� */
	dlinklist q,p=L->next; /* pָ���һ����� */
	while(p!=L) /* pû����ͷ */
	{
		q=p->next;
		free(p);
		p=q;
	}
	L->next=L->prior=L; /* ͷ��������ָ�����ָ������ */
	return OK;
}

status empty_dlist(dlinklist L)
{
	/* ��ʼ���������Ա�L�Ѵ��ڡ������������LΪ�ձ��򷵻�TRUE�����򷵻�FALSE */
	if(L->next==L&&L->prior==L)
	  return TRUE;
	else
	  return FALSE;
}

int length_dlist(dlinklist L)
{
	/* ��ʼ������L�Ѵ��ڡ��������������L������Ԫ�ظ��� */
	int i=0;
	dlinklist p=L->next; /* pָ���һ����� */
	while(p!=L) /* pû����ͷ */
	{
		i++;
		p=p->next;
	}
	return i;
}

status get_delem(dlinklist L,int i,elem_type *e)
{
	/* ����i��Ԫ�ش���ʱ,��ֵ����e������OK,���򷵻�ERROR */
	int j=1; /* jΪ������ */
	dlinklist p=L->next; /* pָ���һ����� */
	while(p!=L&&j<i) /* ˳ָ��������,ֱ��pָ���i��Ԫ�ػ�pָ��ͷ��� */
	{
		p=p->next;
		j++;
	}
	if(p==L||j>i) /* ��i��Ԫ�ز����� */
		return ERROR;
	*e=p->data; /* ȡ��i��Ԫ�� */
	return OK;
}

int locate_delem(dlinklist L,elem_type e,status(*compare)(elem_type,elem_type))
{
	/* ��ʼ������L�Ѵ��ڣ�compare()������Ԫ���ж����� */
	/* �������������L�е�1����e�����ϵcompare()������Ԫ�ص�λ�� */
	/*           ������������Ԫ�ز����ڣ��򷵻�ֵΪ0 */
	int i=0;
	dlinklist p=L->next; /* pָ���1��Ԫ�� */
	while(p!=L)
	{
		i++;
		if(compare(p->data,e)) /* �ҵ�����������Ԫ�� */
			return i;
		p=p->next;
	}
	return 0;
}

status locate_delem(dlinklist L,elem_type cur_e,elem_type *pre_e)
{ 
	/* �����������cur_e��L������Ԫ�أ��Ҳ��ǵ�һ��������pre_e��������ǰ���� */
	/*           �������ʧ�ܣ�pre_e�޶��� */
	dlinklist p=L->next->next; /* pָ���2��Ԫ�� */
	while(p!=L) /* pû����ͷ */
	{
	  if(p->data==cur_e)
	  {
		*pre_e=p->prior->data;
		return TRUE;
	  }
	  p=p->next;
	}
	return FALSE;
}

status next_delem(dlinklist L,elem_type cur_e,elem_type *next_e)
{ 
	/* �����������cur_e��L������Ԫ�أ��Ҳ������һ��������next_e�������ĺ�̣� */
	/*           �������ʧ�ܣ�next_e�޶��� */
	dlinklist p=L->next->next; /* pָ���2��Ԫ�� */
	while(p!=L) /* pû����ͷ */
	{
	  if(p->prior->data==cur_e)
	  {
		*next_e=p->data;
		return TRUE;
	  }
	  p=p->next;
	}
	return FALSE;
}

dlinklist get_delemp(dlinklist L,int i) /* ��� */
{
	/* ��˫������L�з��ص�i��Ԫ�ص�λ��ָ��(�㷨2.18��2.19Ҫ���õĺ���) */
	int j;
	dlinklist p=L;
	for(j=1;j<=i;j++)
	  p=p->next;
	return p;
}

status insert_dlist(dlinklist L,int i,elem_type e) /* �Ľ��㷨2.18 */
{ 
	/* �ڴ�ͷ����˫��ѭ�����Ա�L�е�i��λ��֮ǰ����Ԫ��e��i�ĺϷ�ֵΪ1��i�ܱ�+1 */
	dlinklist p,s;
	if(i<1||i>length_dlist(L)+1) /* iֵ���Ϸ� */
	  return ERROR;
	p=get_delemp(L,i-1); /* ��L��ȷ����i-1��Ԫ�ص�λ��ָ��p */
	if(!p) /* p=NULL,����i-1��Ԫ�ز����� */
	  return ERROR;
	s=(dlinklist)malloc(sizeof(dnode));
	if(!s)
	  return OVERFLOW;
	s->data=e; /* �ڵ�i-1��Ԫ��֮����� */
	s->prior=p;
	s->next=p->next;
	p->next->prior=s;
	p->next=s;
	return OK;
}

status delete_dlist(dlinklist L,int i,elem_type *e) /* �㷨2.19 */
{
	/* ɾ����ͷ����˫��ѭ�����Ա�L�ĵ�i��Ԫ��,i�ĺϷ�ֵΪ1��i�ܱ�+1 */
	dlinklist p;
	if(i<1||i>length_dlist(L)) /* iֵ���Ϸ� */
	  return ERROR;
	p=get_delemp(L,i);  /* ��L��ȷ����i��Ԫ�ص�λ��ָ��p */
	if(!p) /* p=NULL,����i��Ԫ�ز����� */
	  return ERROR;
	*e=p->data;
	p->prior->next=p->next;
	p->next->prior=p->prior;
	free(p);
	return OK;
}

void traverse_dlist(dlinklist L,void(*visit)(elem_type))
{ 
	/* ��˫��ѭ�����Ա�L��ͷ������,�����ÿ������Ԫ�ص��ú���visit() */
	dlinklist p=L->next; /* pָ��ͷ��� */
	while(p!=L)
	{
	  visit(p->data);
	  p=p->next;
	}
	printf("\n");
}

void traverse_back_dlist(dlinklist L,void(*visit)(elem_type))
{
	/* ��˫��ѭ�����Ա�L��ͷ������,�����ÿ������Ԫ�ص��ú���visit()����� */
	dlinklist p=L->prior; /* pָ��β��� */
	while(p!=L)
	{
	  visit(p->data);
	  p=p->prior;
	}
	printf("\n");
}


status dlistcompare(elem_type c1,elem_type c2) /* ����Ԫ���ж�����(�ж����) */
{
	if(c1==c2)
	  return TRUE;
	else
	  return FALSE;
}

void visit_dlist(elem_type c) /* traverse_dlist()���õĺ���(����һ��) */
{
	printf("%d ",c);
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//  dlinklist L;
//   int i,n;
//   status j;
//   elem_type e;
//   init_dlist(&L);
//   for(i=1;i<=5;i++)
//     insert_dlist(L,i,i); /* �ڵ�i�����֮ǰ����i */
//   printf("�����������");
//   traverse_dlist(L,visit_dlist); /* ������� */
//   printf("�����������");
//   traverse_back_dlist(L,visit_dlist); /* ������� */
//   n=2;
//   delete_dlist(L,n,&e); /* ɾ�����ͷŵ�n����� */
//   printf("ɾ����%d����㣬ֵΪ%d��������Ϊ��",n,e);
//   traverse_dlist(L,visit_dlist); /* ������� */
//   printf("�����Ԫ�ظ���Ϊ%d\n",length_dlist(L));
//   printf("�����Ƿ�գ�%d(1:�� 0:��)\n",empty_dlist(L));
//   clear_dlist(L); /* ������� */
//   printf("��պ������Ƿ�գ�%d(1:�� 0:��)\n",empty_dlist(L));
//   for(i=1;i<=5;i++)
//     insert_dlist(L,i,i); /* ���²���5����� */
//   traverse_dlist(L,visit_dlist); /* ������� */
//   n=3;
//   j=get_delem(L,n,&e); /* ������ĵ�n��Ԫ�ظ�ֵ��e */
//   if(j)
//     printf("����ĵ�%d��Ԫ��ֵΪ%d\n",n,e);
//   else
//     printf("�����ڵ�%d��Ԫ��\n",n);
//   n=4;
//   i=locate_delem(L,n,dlistcompare);
//   if(i)
//     printf("����%d��Ԫ���ǵ�%d��\n",n,i);
//   else
//     printf("û�е���%d��Ԫ��\n",n);
//   j=locate_delem(L,n,&e);
//   if(j)
//     printf("%d��ǰ����%d\n",n,e);
//   else
//     printf("������%d��ǰ��\n",n);
//   j=next_delem(L,n,&e);
//   if(j)
//     printf("%d�ĺ����%d\n",n,e);
//   else
//     printf("������%d�ĺ��\n",n);
//   destroy_dlist(&L);
//}
