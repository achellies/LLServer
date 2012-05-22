#include "defines.h"

typedef int elem_type;

/* ������� */
typedef struct lnode 
{
   elem_type data;
   struct lnode *next;
} lnode,*link,*pos;

/* �������� */
typedef struct linklist
{
	/* �ֱ�ָ�����������е�ͷ�������һ����� */
    link head,tail; 
	/* ָʾ��������������Ԫ�صĸ��� */
    int len; 
} linklist;

status create_node(link *p,elem_type e);

void free_node(link *p);

status init_list(linklist *L);

status clear_list(linklist *L);

status destroy_list(linklist *L);

status insert_first(linklist *L,link h,link s);

status delete_first(linklist *L, link h, link *q);

status append(linklist *L,link s);

pos prior_pos(linklist L,link p);

status remove(linklist *L,link *q);

status insert_before(linklist *L,link *p,link s);

status insert_after(linklist *L,link *p,link s);

status set_curelem(link p,elem_type e);

elem_type get_curelem(link p);

status empty_list(linklist L);

int list_length(linklist L);

pos list_head(linklist L);

pos get_last(linklist L);

pos next_pos(link p);

status locate_pos(linklist L, int i, link *p);

pos locate_elem(linklist L,elem_type e,status (*compare)(elem_type,elem_type));

status list_traverse(linklist L, void(*visit)(elem_type));

status order_insert(linklist *L, elem_type e, int (*comp)(elem_type,elem_type));

status locate_elemp(linklist L, elem_type e,pos *q, int(*compare)(elem_type,elem_type));

status compare(elem_type c1,elem_type c2);

int cmp(elem_type a,elem_type b);

void visit(elem_type c);
