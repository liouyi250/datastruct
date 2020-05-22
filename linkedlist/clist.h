#ifndef CLIST_H
#define CLIST_H

typedef struct CListEle_
{
    void *data;
    struct ClistEle_ *next;
}CListEle;

typedef struct CList_
{
    int size;
    int (*match)(const void *key1,const void *key2);
    void (*destroy)(void *data);
    CListEle *head;
}CList;

void clist_init(CList *list,void (*destroy)(void *data));
void clist_destroy(CList *list);
int clist_ins_next(CList *list,CListEle *element,const void *data);
int clist_rem_next(CList *list,CListEle *element,void **data);

#define clist_size(list) ((list)->size)
#define clist_head(list) ((list)->head)
#define clist_data(ele) ((ele)->data)
#define clist_next(ele) (ele)->next)
#endif