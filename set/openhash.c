#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "openhash.h"

int match(const char *,const char*);
unsigned int BKDRHash(const char *);
int h1(const char *);
int h2(const char *);

/*
计算positions，返回2次幂,仿java的hashmap
*/
__uint32_t ohtbl_init_size(int positions)
{
    int n=31;
    if(positions<=0)
    {
         if(positions==0){
             positions=32;
         }else{
             positions=0;
         }
    }
        
    if(positions>=66536)
    {
        positions >>= 16;
        n-=16;
    }
    if(positions>=256)
    {
        positions >>= 8;
        n-=8;
    }
    if(positions>=16)
    {
        positions >>= 4;
        n-=4;
    }
    if(positions>=4)
    {
        positions >>= 2;
        n-=2;
    }
    n=n-(positions>>1);
    __uint32_t b=-1 & 0xFFFFFFFF;
    b >>= n;
    positions=b+1;
    return positions;
}

/*
初始化哈希表，并计算positions的值
*/
int ohtbl_init(OHTbl *htbl,int positions,int (*h1)(const void* key),
                int (*h2)(const void *key2),int (*match)(const void *key1,const void *key2),
                void (*destroy)(void *data))
{
    ohtbl_init_size(positions);
    htbl->destroy=destroy;
    htbl->h1=h1;
    htbl->h2=h2;
    htbl->match=match;
    htbl->positions=positions;
    htbl->size=0;
    htbl->table=(void**)malloc(sizeof(void *)*positions);
    if(htbl->table==NULL) return -1;
    for(int i=0;i<positions;i++)
    {
        htbl->table[i]=NULL;
    }
    return 0;
}
/*
销毁hashtable
*/
void ohtbl_destroy(OHTbl *htbl)
{
    void **table=htbl->table;
    void *data;
    while(ohtbl_size(htbl)>0)
    {
        ohtbl_remove(htbl,&data);
        if(htbl->destroy!=NULL)
            htbl->destroy(data);
    }
    free(table);
    return ;
}
/*
插入元素
*/
int ohtbl_insert(OHTbl *htbl,const void *data)
{
    //当元素满了时，应该对table进行扩容，但是扩容之后导致元素的槽位发生了变化，这里简单返回
    if(htbl->size >= htbl->positions) {
        return -1;
    }
    int position,i;
    void *ele=(void*)data;
    for(int i=0;i<htbl->positions;i++)
    {
        position=(htbl->h1(data)+i*htbl->h2(data)) % htbl->positions;
        if(htbl->table[position]==NULL){
            printf("Insert data:%s,Position: %d\n",(const char*)data,position);
            htbl->table[position]=ele;
            htbl->size++;
            return 0;
        }else if(htbl->match(htbl->table[position],(const void*)data))
        {
            return -1;
        }
    }
    return -1;
}
/*
删除元素
*/
int ohtbl_remove(OHTbl *htbl,void **data)
{
    if(ohtbl_size(htbl)==0) return -1;
    int position,i;
    for(i=0;i<htbl->positions;i++)
    {
        position=(htbl->h1(*data)+i*htbl->h2(*data)) % htbl->positions;
        if(htbl->table[position]==NULL){
            continue;
        }else if(htbl->match(htbl->table[position],*data)){
            printf("Remove data:%s,Position: %d\n",(const char*)*data,position);
            *data=htbl->table[position];
            htbl->table[position]=NULL;
            htbl->size--;
            return 0;
        }
    }
    return -1;
}
/*
查看hashtable中该关键字是否存在
*/
int ohtbl_lookup(OHTbl *htbl,void **data)
{
    int i,position;
    for(int i=0;i<htbl->positions;i++)
    {
        position=(htbl->h1(*data)+i*htbl->h2(*data)) % htbl->positions;
        //该槽位上没有元素
        if(htbl->table[position]==NULL){
            return -1;
            //该槽位上已有元素，返回当前元素在table中的位置
        }else if(htbl->match(htbl->table[position],*data)){
            *data=htbl->table[position];
            return position;
        }
    }
    return -1;
}

static int positions = 8;
int main(int argc,char *argv[])
{
    
    char *str[20]={"SHE","SHE","HE","HE","THANK",
    "YOU","I","LOVE","DATA","STRUCT","DATA","ALGORITHM",
    "PERFECT","THIS","THAT","THERE","HEY","AGREE","WORLD","YOU"};
    OHTbl htbl;
    ohtbl_init(&htbl,positions,h1,h2,match,NULL);
    for(int i=0;i<20;i++)
    {
        ohtbl_insert(&htbl,(const void*)str[i]);
    }
    printf("table size: %d\n",ohtbl_size(&htbl));
    for(int i=0;i<20;i++)
    {
        ohtbl_remove(&htbl,(void**)&str[i]);
    }
    printf("table size: %d\n",ohtbl_size(&htbl));

    ohtbl_destroy(&htbl);
    
   
}


int match(const char *str1,const char *str2)
{
    return strcmp(str1,str2)==0 ? 1 : 0;
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
//使用静态变量positions
int h1(const char *str)
{
    __uint32_t key=BKDRHash(str);
    return key % positions;
}

int h2(const char *str)
{
    __uint32_t key=BKDRHash(str);
    return key % (positions -1);
}