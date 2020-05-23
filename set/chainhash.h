#ifndef CHAINHASH_H
#define CHAINHASH_H
/*
哈希表分类
链式哈希表：将数据存储在桶中的哈希表，每个桶是一个链表，且链表的容量能够随着冲突的增加而增大
开地址哈希表：将数据存储在表本身，而不是桶中的哈希表。

哈希函数
是哈希算法的核心问题，将键随机分散到表中，使冲突最小化，返回一个整型数据。
常用的方式：
取余法，h(k)=k mod m;k为需要存储的键的个数，m为桶的个数，最好是一个素数
乘法:h(k)=m(kA mod 1)，通常A取黄金分割数，m为桶的个数，k意义同上

冲突解决
链式哈希表以固有方式解决。
*/
#include "../linkedlist/list.h"
typedef struct element_
{
    void *data;
    struct element_ *next;
}Element;

//链式哈希表
typedef struct CHTbl_
{
    //桶的个数
    int buckets;
    //哈希函数
    int (*h)(const void *key);
    int (*match)(const void *key,const void *key2);
    void (*destroy)(void *data);
    int size;
    //每个链表的头指针
    List *table;
}CHTbl;

int chtbl_init(CHTbl *htbl,int buckets,int (*h)(const void *key),int (*match)(const void* key1,const void *key2),void (*destroy)(void *data));
int chtbl_insert(CHTbl *htbl,const void *data);
int chtbl_remove(CHTbl *htbl, void **data);
int chtbl_looup(const CHTbl *htbl,void **data);
#define chtbl_size(htbl) ((htbl)->size)

#endif