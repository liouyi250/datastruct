#ifndef DLIST_H
#define DLIST_H

typedef struct DListEle_
{
    void *data;
    struct DListEle_ *prev;
    struct DListEle_ *next;
}DListEle;

typedef struct DList_
{
    int size;
    int (*match)(const void *key1,const void* key2);
    void (*destroy)(void *data);
    DListEle *head;
    DListEle *tail;
}DList;

void dlist_init(DList* list,void (*destroy)(void* data));
void dlist_destroy(DList *list);
int dlist_ins_next(DList *list,DListEle *element,const void *data);
int dlist_ins_prev(DList *list,DListEle *element,const void *data);
void dlist_empty_ins(DList *list,DListEle *insetEle);
int dlist_remove(DList *list,DListEle *element,void **data);
#define dlist_size(list) ((list)->size)
#define dlist_head(list) ((list)->head)
#define dlist_tail(list) ((list)->tail)
#define dlist_is_head(ele)  ((ele)->prev==NULL ? 1 :0)
#define dlist_is_tail(ele)  ((ele)->next==NULL ? 1:0)
#define dlist_data(ele)  ((ele)->data)
#define dlist_prev(ele) ((ele)->prev)
#define dlist_next(ele) ((ele)->next)
#endif