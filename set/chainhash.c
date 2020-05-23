#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "chainhash.h"
/*
链式哈希表每个桶内的元素不宜过长，最好控制在5个以内
*/
//使用乘法，假设有27个元素，有10个桶
int h(const void *key)
{
    int k=(int)key;
    int m=400;
    float A=0.618;
    float f=m*(k*A-(int)(k*A));
    return (int)f;
}
/*
字符串BKDRHash算法
*/
unsigned int BKDRHash(const char* str)
{
    unsigned int seed=131;//31 131 1313 13131 131313
    unsigned int hash=0;
    while(*str)
    {
        hash=hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

int match(const void* key1,const void *key2){
    return (int)key1 == (int)key2;
}

int chtbl_init(CHTbl *htbl,int buckets,int (*h)(const void *key),int (*match)(const void* key1,const void *key2),void (*destroy)(void *data))
{
    htbl->size=0;
    if(buckets<=0) return -1;
    htbl->table=(List*)malloc(sizeof(List)*buckets);
    if(htbl->table==NULL) return -1;
    for(int i=0;i<buckets;i++)
        list_init(htbl->table+i,destroy);
    htbl->h=h;
    htbl->destroy=destroy;
    htbl->match=match;
    return 0;
}

int chtbl_insert(CHTbl *htbl,const void *data)
{
    int n=htbl->h(data);
    //在表中已经存在该元素
    if(chtbl_looup(htbl,(const void**)&data)) return -1;
    List *list=htbl->table+n;
    //在链表结尾添加元素
    list_ins_next(list,list->tail,data);
    htbl->size++;
    return 0;
}

int chtbl_remove(CHTbl *htbl, void **data)
{
    //在表中没有该元素
    if(chtbl_looup(htbl,(const void**)&data)==0) return -1;
    //计算在桶中的位置
    int n=htbl->h(*data);
    //获的该桶的首指针
    List *list=htbl->table+n;
    Element *prev=list->head;
    Element *curr=list->head;
    for(int i=0;i<list_size(list);i++)
    {
        if(list->match((const void*)curr->data,(const void**)&data))
        {
            break;
        }
        prev=curr;
        curr=curr->next;
    }
    if(curr==list->head)
    {
        list->head=curr->next;
    }
    else
    {
        prev->next=curr->next;
    }
    list->size--;
    htbl->size--;
    free(curr);
    curr=NULL;
    return 0;
}
int chtbl_looup(const CHTbl *htbl,void **data)
{
    int n=htbl->h(*data);
    Element *element=htbl->table[n].head;
    short flag=0;
    for(int i=0;i<list_size(htbl->table+n);i++)
    {
        if(htbl->match((const void*)element->data,(const void*)*data))
        {
            flag=1;
            break;
        }
        element=element->next;
    }
    return flag==1 ? 1 : 0;
}

#define Len 1000
int main(int argc, char const *argv[])
{
    CHTbl htbl;
    clock_t start,end;
    chtbl_init(&htbl,400,h,match,NULL);
    srand(time(NULL));
    int arr[Len];
    start=clock();
    for(int i=0;i<Len;i++)
    {
        arr[i]=i;
        chtbl_insert(&htbl,arr[i]);
    }
    int *t;
    for(int i=0;i<Len;i++)
    {
        t=arr+i;
        chtbl_remove(&htbl,(void**)&t);
    }
    end=clock();
    printf("%f\n",difftime(end,start));
    return 0;
}
