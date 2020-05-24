#ifndef OPENHASH_H
#define OPENHASH_H
/*
开地址哈希表
负载因子a=n/m，n为元素个数，m为散列元素的槽位个数,a越大，哈希表的效率越低
开地址哈希表自身没有冲突解决机制，当有冲突发生时，需要探查这个表，直到找到一个可以放置元素的位置

冲突解决方法：
线性探查：h(k,i)=(h1(k)+i) mod m,h1(k)是哈希函数，尽可能将元素随机均匀分配到表中，优点是简单，对m没有限制，缺点是不能近似均匀散列
双散列：h(k,i)=(h1(k)+ih2(k)) mod m，h1和h2是两个哈希函数。有两种方法保证均匀，第一个是m必须是2次幂，h2返回一个奇数。第二个是m是一个素数，h2返回值为1<=h2(k)<=m-1
通常情况下令h1(k)=k mod m，h2(k)=1+(k mod m1)，m1类似等于m-1或m-2.有点是能较好的元素分布，缺点是必须限制m的值。
*/

typedef struct  OHTbl_
{
    int positions;
    int (*h1)(const void *key);
    int (*h2)(const void *key);
    int (*match)(const void *key1,const void *key2);
    void (*destroy)(void *data);
    int size;
    void **table;
}OHTbl;

int ohtbl_init(OHTbl *htbl,int positions,int (*h1)(const void* key),
                int (*h2)(const void *key2),int (*match)(const void *key1,const void *key2),
                void (*destroy)(void *data));
/*
计算positions，返回2次幂,仿java的hashmap
*/
__uint32_t ohtbl_init_size(int positions);

void ohtbl_destroy(OHTbl *htbl);
int ohtbl_insert(OHTbl *htbl,const void *data);
int ohtbl_remove(OHTbl *htbl,void **data);
int ohtbl_lookup(OHTbl *htbl,void **data);
#define ohtbl_size(htbl) ((htbl)->size)

#endif