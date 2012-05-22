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
#include "linklist.h"

status create_node(link *p,elem_type e)
{ 
	/* ������pָ���ֵΪe�Ľ�㣬������OK��������ʧ�ܡ��򷵻�ERROR */
	*p=(link)malloc(sizeof(lnode));
	if(!*p)
	   return ERROR;
	(*p)->data=e;
	return OK;
}

void free_node(link *p)
{ /* �ͷ�p��ָ��� */
	free(*p);
	*p=NULL;
}

status init_list(linklist *L)
{ /* ����һ���յ��������� */
	link p;
	p=(link)malloc(sizeof(lnode)); /* ����ͷ��� */
	if(p)
	{
		p->next=NULL;
		(*L).head=(*L).tail=p;
		(*L).len=0;
		return OK;
	}
	else
		return ERROR;
}

status clear_list(linklist *L)
{ /* ����������L����Ϊ�ձ����ͷ�ԭ����Ľ��ռ� */
	link p,q;
	if((*L).head!=(*L).tail)/* ���ǿձ� */
	{
	  p=q=(*L).head->next;
	  (*L).head->next=NULL;
	  while(p!=(*L).tail)
	  {
		p=q->next;
		free(q);
		q=p;
	  }
	  free(q);
	  (*L).tail=(*L).head;
	  (*L).len=0;
	}
	return OK;
}

status destroy_list(linklist *L)
{ 
	/* ������������L��L���ٴ��� */
	clear_list(L); /* ������� */
	free_node(&(*L).head);
	(*L).tail=NULL;
	(*L).len=0;
	return OK;
}

status insert_first(linklist *L,link h,link s) /* �β�����L,��Ϊ���޸�L */
{ 
	/* hָ��L��һ����㣬��h����ͷ��㣬��s��ָ�������ڵ�һ�����֮ǰ */
	s->next=h->next;
	h->next=s;
	if(h==(*L).tail) /* hָ��β��� */
	  (*L).tail=h->next; /* �޸�βָ�� */
	(*L).len++;
	return OK;
}

status delete_first(linklist *L, link h, link *q) /* �β�����L,��Ϊ���޸�L */
{ 
	/* hָ��L��һ����㣬��h����ͷ��㣬ɾ�������еĵ�һ����㲢��q���ء� */
	 /* ������Ϊ��(hָ��β���)��q=NULL������FALSE */
	 *q=h->next;
	 if(*q) /* ����ǿ� */
	 {
		h->next=(*q)->next;
		if(!h->next) /* ɾ��β��� */
		  (*L).tail=h; /* �޸�βָ�� */
		(*L).len--;
		return OK;
	 }
	 else
	    return FALSE; /* ����� */
}

status append(linklist *L,link s)
{ 
	/* ��ָ��s(s->dataΪ��һ������Ԫ��)��ָ(�˴���ָ������,��NULL��β)�� */
	/* һ�������������������L�����һ�����֮��,���ı�����L��βָ��ָ���� */
	/* ��β��� */
	int i=1;
	(*L).tail->next=s;
	while(s->next)
	{
	  s=s->next;
	  i++;
	}
	(*L).tail=s;
	(*L).len+=i;
	return OK;
}

pos prior_pos(linklist L,link p)
{ 
	/* ��֪pָ����������L�е�һ����㣬����p��ָ����ֱ��ǰ����λ�� */
	/* ����ǰ�����򷵻�NULL */
	link q;
	q=L.head->next;
	if(q==p) /* ��ǰ�� */
		return NULL;
	else
	{
		while(q->next!=p) /* q����p��ֱ��ǰ�� */
		  q=q->next;
		return q;
	}
}

status remove(linklist *L,link *q)
{ 
	/* ɾ����������L�е�β��㲢��q���أ��ı�����L��βָ��ָ���µ�β��� */
	link p=(*L).head;
	if((*L).len==0) /* �ձ� */
	{
		*q=NULL;
		return FALSE;
	}
	while(p->next!=(*L).tail)
		p=p->next;
	*q=(*L).tail;
	p->next=NULL;
	(*L).tail=p;
	(*L).len--;
	return OK;
}

status insert_before(linklist *L,link *p,link s)
{
	/* ��֪pָ����������L�е�һ����㣬��s��ָ��������p��ָ���֮ǰ�� */
	/* ���޸�ָ��pָ���²���Ľ�� */
	link q;
	q=prior_pos(*L,*p); /* q��p��ǰ�� */
	if(!q) /* p��ǰ�� */
		q=(*L).head;
	s->next=*p;
	q->next=s;
	*p=s;
	(*L).len++;
	return OK;
}

status insert_after(linklist *L,link *p,link s)
{ 
	/* ��֪pָ����������L�е�һ����㣬��s��ָ��������p��ָ���֮�� */
	/* ���޸�ָ��pָ���²���Ľ�� */
	if(*p==(*L).tail) /* �޸�βָ�� */
		(*L).tail=s;
	s->next=(*p)->next;
	(*p)->next=s;
	*p=s;
	(*L).len++;
	return OK;
}

status set_curelem(link p,elem_type e)
{
	/* ��֪pָ�����������е�һ����㣬��e����p��ָ���������Ԫ�ص�ֵ */
    p->data=e;
    return OK;
}

elem_type get_curelem(link p)
{ 
	/* ��֪pָ�����������е�һ����㣬����p��ָ���������Ԫ�ص�ֵ */
    return p->data;
}

status empty_list(linklist L)
{ 
	/* ����������LΪ�ձ��򷵻�TRUE�����򷵻�FALSE */
	if(L.len)
		return FALSE;
	else
		return TRUE;
}

int list_length(linklist L)
{ 
	/* ������������L��Ԫ�ظ��� */
    return L.len;
}

pos list_head(linklist L)
{ 
	/* ������������L��ͷ����λ�� */
    return L.head;
}

pos get_last(linklist L)
{
	/* ������������L�����һ������λ�� */
    return L.tail;
}

pos next_pos(link p)
{ 
	/* ��֪pָ����������L�е�һ����㣬����p��ָ����ֱ�Ӻ�̵�λ�� */
    /* ���޺�̣��򷵻�NULL */
    return p->next;
}

status locate_pos(linklist L,int i,link *p)
{
	/* ����pָʾ��������L�е�i������λ�ã�������OK��iֵ���Ϸ�ʱ����ERROR */
	/* i=0Ϊͷ��� */
	int j;
	if(i<0||i>L.len)
		return ERROR;
	else
	{
		*p=L.head;
		for(j=1;j<=i;j++)
		  *p=(*p)->next;
		return OK;
	}
}

pos locate_elem(linklist L,elem_type e,status (*compare)(elem_type,elem_type))
{ 
	/* ������������L�е�1����e���㺯��compare()�ж���ϵ��Ԫ�ص�λ�ã� */
	/* ��������������Ԫ�أ��򷵻�NULL */
	link p=L.head;
	do
		p=p->next;
	while(p&&!(compare(p->data,e))); /* û����β��û�ҵ������ϵ��Ԫ�� */
	return p;
}

status list_traverse(linklist L,void(*visit)(elem_type))
{ 
	/* ���ζ�L��ÿ������Ԫ�ص��ú���visit()��һ��visit()ʧ�ܣ������ʧ�� */
	link p=L.head->next;
	int j;
	for(j=1;j<=L.len;j++)
	{
		visit(p->data);
		p=p->next;
	}
	printf("\n");
	return OK;
}

status order_insert(linklist *L,elem_type e,int (*comp)(elem_type,elem_type))
{ 
	/* ��֪LΪ��������������Ԫ��e���ǽ��������L�С�������һԪ����ʽ�� */
	link o,p,q;
	q=(*L).head;
	p=q->next;
	while(p!=NULL&&comp(p->data,e)<0) /* p���Ǳ�β��Ԫ��ֵС��e */
	{
		q=p;
		p=p->next;
	}
	o=(link)malloc(sizeof(lnode)); /* ���ɽ�� */
	o->data=e; /* ��ֵ */
	q->next=o; /* ���� */
	o->next=p;
	(*L).len++; /* ����1 */
	if(!p) /* ���ڱ�β */
		(*L).tail=o; /* �޸�β��� */
	return OK;
}

status locate_elemp(linklist L,elem_type e,pos *q,int(*compare)(elem_type,elem_type))
{ 
	/* ����������L�д�����e�����ж�����compare()ȡֵΪ0��Ԫ�أ���qָʾL�� */
	/* ��һ��ֵΪe�Ľ���λ�ã�������TRUE������qָʾ��һ����e�����ж����� */
	/* compare()ȡֵ>0��Ԫ�ص�ǰ����λ�á�������FALSE��������һԪ����ʽ�� */
	link p=L.head,pp;
	do
	{
		pp=p;
		p=p->next;
	}while(p&&(compare(p->data,e)<0)); /* û����β��p->data.expn<e.expn */

	if(!p||compare(p->data,e)>0) /* ����β��compare(p->data,e)>0 */
	{
		*q=pp;
		return FALSE;
	}
	else /* �ҵ� */
	{
		*q=p;
		return TRUE;
	}
}



status compare(elem_type c1,elem_type c2) /* c1����c2 */
{
	if(c1==c2)
		return TRUE;
	else
		return FALSE;
}

int cmp(elem_type a,elem_type b)
{
	  /* ����a<��=��>b,�ֱ𷵻�-1��0��1 */
	if(a==b)
		return 0;
	else
		return (a-b)/abs(a-b);
}

void visit(elem_type c)
{
	printf("%d ",c);
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//   link p,h;
//   linklist L;
//   status i;
//   int j,k;
//   i=init_list(&L);
//   if(!i) /* ��ʼ���յ����Ա�L���ɹ� */
//     exit(FALSE); /* �˳��������� */
//   for(j=1;j<=2;j++)
//   {
//     create_node(&p,j); /* ������pָ��ֵΪj�Ľ�� */
//     insert_first(&L,L.tail,p); /* ���ڱ�β */
//   }
//   order_insert(&L,0,cmp); /* ��������������ͷ */
//   for(j=0;j<=3;j++)
//   {
//     i=locate_elemp(L,j,&p,cmp);
//     if(i)
//       printf("��������ֵΪ%d��Ԫ�ء�\n",p->data);
//     else
//       printf("������û��ֵΪ%d��Ԫ�ء�\n",j);
//   }
//   printf("�������");
//   list_traverse(L,visit); /* ���L */
//   for(j=1;j<=4;j++)
//   {
//     printf("ɾ����ͷ��㣺");
//     delete_first(&L,L.head,&p); /* ɾ��L���׽�㣬����p���� */
//     if(p)
//       printf("%d\n",get_curelem(p));
//     else
//       printf("��գ��޷�ɾ�� p=%u\n",p);
//   }
//   printf("L�н�����=%d L�Ƿ�� %d(1:�� 0:��)\n",list_length(L),empty_list(L));
//   create_node(&p,10);
//   p->next=NULL; /* β��� */
//   for(j=4;j>=1;j--)
//   {
//     create_node(&h,j*2);
//     h->next=p;
//     p=h;
//   } /* hָ��һ��5����㣬��ֵ������2 4 6 8 10 */
//   append(&L,h); /* �ѽ��h��������������L�����һ�����֮�� */
//   order_insert(&L,12,cmp); /* ��������������βͷ */
//   order_insert(&L,7,cmp); /* ���������������м� */
//   printf("�������");
//   list_traverse(L,visit); /* ���L */
//   for(j=1;j<=2;j++)
//   {
//     p=locate_elem(L,j*5,compare);
//     if(p)
//       printf("L�д���ֵΪ%d�Ľ�㡣\n",j*5);
//     else
//       printf("L�в�����ֵΪ%d�Ľ�㡣\n",j*5);
//   }
//   for(j=1;j<=2;j++)
//   {
//     locate_pos(L,j,&p); /* pָ��L�ĵ�j����� */
//     h=prior_pos(L,p); /* hָ��p��ǰ�� */
//     if(h)
//       printf("%d��ǰ����%d��\n",p->data,h->data);
//     else
//       printf("%dûǰ����\n",p->data);
//   }
//   k=list_length(L);
//   for(j=k-1;j<=k;j++)
//   {
//     locate_pos(L,j,&p); /* pָ��L�ĵ�j����� */
//     h=next_pos(p); /* hָ��p�ĺ�� */
//     if(h)
//       printf("%d�ĺ����%d��\n",p->data,h->data);
//     else
//       printf("%dû��̡�\n",p->data);
//   }
//   printf("L�н�����=%d L�Ƿ�� %d(1:�� 0:��)\n",list_length(L),empty_list(L));
//   p=get_last(L); /* pָ�����һ����� */
//   set_curelem(p,15); /* �����һ������ֵ��Ϊ15 */
//   printf("��1��Ԫ��Ϊ%d ���1��Ԫ��Ϊ%d\n",get_curelem(list_head(L)->next),get_curelem(p));
//   create_node(&h,10);
//   insert_before(&L,&p,h); /* ��10�嵽β���֮ǰ��pָ���½�� */
//   p=p->next; /* p�ָ�Ϊβ��� */
//   create_node(&h,20);
//   insert_after(&L,&p,h); /* ��20�嵽β���֮�� */
//   k=list_length(L);
//   printf("����ɾ����β��㲢�����ֵ��");
//   for(j=0;j<=k;j++)
//   {
//     i=remove(&L,&p);
//     if(!i) /* ɾ�����ɹ� */
//       printf("ɾ�����ɹ� p=%u\n",p);
//     else
//       printf("%d ",p->data);
//   }
//   create_node(&p,29); /* �ؽ�����1�����(29)������ */
//   insert_first(&L,L.head,p);
//   destroy_list(&L); /* ������������L */
//   printf("������������L֮��: L.head=%u L.tail=%u L.len=%d\n",L.head,L.tail,L.len);
//}
