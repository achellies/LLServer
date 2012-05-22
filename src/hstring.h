#include "defines.h"

/*���Ķѷ���洢 */
typedef struct
{
  char *ch; /* ���Ƿǿմ�,�򰴴�������洢��,����chΪNULL */
  int length; /* ������ */
} hstring;

hstring hs_init(char *data);

void hs_init(hstring *pstr);

status hs_assign(hstring *pstr,char *chars);

status hs_copy(hstring *pstr,hstring s);

status hs_empty(hstring s);

int hs_compare(hstring s1,hstring s2);

int hs_length(hstring s);

status hs_clear(hstring *pstr);

status hs_concat(hstring *pstr,hstring s1,hstring s2);

status hs_substring(hstring *pstr, hstring s, int pos, int len);

int hs_find(hstring s1,hstring s2,int pos);

int hs_findfirst(hstring s1, hstring s2);

status hs_insert(hstring *pstr,int pos, hstring s);

status hs_delete(hstring *pstr,int pos,int len);

status hs_replace(hstring *s, hstring s1, hstring s2);

void hs_print(hstring s);

status hs_startwith(hstring s1,hstring s2);

status hs_endwith(hstring s1,hstring s2);

int hs_findlast(hstring s1, hstring s2);

int hs_findcount(hstring s1, hstring s2);
