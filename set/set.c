#include <stdio.h>
#include <stdlib.h>
#include "set.h"

int match(const void* key1,const void* key2)
{
    return (int)key1 == (int)key2;
}


void set_init(Set *set,int (*match)(const void *key1,const void *key2),void (*destroy)(void *data))
{
    set->size=0;
    set->head=NULL;
    set->tail=NULL;
    set->destroy=destroy;
    set->match=match;
}
int set_insert(Set *set,const void *data)
{
    SetEle *element=(SetEle*)malloc(sizeof(SetEle));
    if(element==NULL) return -1;
    element->data=(void*)data;
    //空集合
    if(set_size(set)==0)
    {
        set->head=element;
        set->tail=element;
        element->next=NULL;
    }
    else
    {
        SetEle *tmp=set->head;
        int flag=0;
        //判断当前元素在集合中是否存在
        while(tmp)
        {
            if (set->match((const void*)tmp->data,data)==1)
            {
                flag=1;
                break;
            }
            tmp=tmp->next;
        }
        //被插入元素在集合中存在
        if(flag==1) return -1;
        else
        {
            element->next=NULL;
            set->tail->next=element;
            set->tail=element;
        }
    }
    set->size++;
    return 0;
}


int set_remove(Set *set,void **data)
{
    //空集合
    if(set_size(set)==0) return -1;
    //遍历集合，判断当前元素是否存在
    SetEle *tmp=set->head;
    SetEle *prev=set->head;//要被删除元素的前指针
    int flag=0;
    while(tmp){
        if(set->match((const void*)tmp->data,(const void*)*data))
        {
            flag=1;
            break;
        }
        prev=tmp;
        tmp=tmp->next;
    }
    //被删除元素不存在
    if(flag==0) return -1;
    //被删除元素是头指针
    if(tmp==set->head)
    {
        set->head=tmp->next;
    }
    else
    {
        prev->next=tmp->next;
    }
    //set内的元素是动态分配的;
    if(set->destroy!=NULL)
    {
        set->destroy(*data);
    }
    free(tmp);
    tmp=NULL;
    set->size--;
    return 0;
}
int set_union(Set *setu,const Set *set1,const Set *set2)
{
    set_init(setu,set1->match,set1->destroy);
    setu->head=set1->head;
    setu->size=set1->size;
    SetEle *tmp=set2->head;
    SetEle *setuTail=set1->tail;//setu的尾指针
    while(tmp)
    {
        if(set_is_member(set1,(const void*)tmp->data)==0)
        {
            setuTail->next=tmp;//尾指针指向tmp
            setuTail=setuTail->next;
            setu->tail=setuTail;//修改setu尾指针指向
            setu->size++;
        }
        tmp=tmp->next;
    }
}

int set_difference(Set *setd,const Set *set1,const Set* set2)
{
    set_init(setd,set1->match,set1->destroy);
    setd->head=set1->head;
    setd->tail=set1->tail;
    setd->size=set1->size;
    SetEle *tmp1=setd->head;
    SetEle *prev=setd->head;
    SetEle *tmp2=set2->head;

    while(tmp2)
    {
        if(set_is_member(set1,(const void*)tmp2->data))
        {
            tmp1=set1->head;
            while(tmp1)
            {
                if(set1->match((const void*)tmp1->data,(const void*)tmp2->data))
                {
                    if(tmp1==setd->head)
                    {
                        setd->head=tmp1->next;
                    }
                    else
                    {
                        prev->next=tmp1->next;
                    }
                    setd->size--;
                }
                prev=tmp1;
                tmp1=tmp1->next;
            }
        }
        tmp2=tmp2->next;
    }
    return 0;
}
int set_is_member(const Set *set1,const void *data)
{
    SetEle *tmp=set1->head;
    int i;
    for(i=0;i<set_size(set1);i++)
    {
        if(set1->match((const void*)tmp->data,data)==1)
        {
            return 1;
        }
        tmp=tmp->next;
    }
    return 0;
}
int set_is_subset(const Set *set1,const Set *set2)
{
    SetEle *tmp=set2->head;
    while(tmp)
    {
        if(set_is_member(set1,(const void*)tmp->data)==0){
            return -1;
        }
        tmp=tmp->next;
    }
    return 1;
}
int set_is_equal(const Set *set1,const Set *set2)
{
    if(set_size(set1) != set_size(set2)) return -1;
    SetEle *tmp=set2->head;
    while(tmp)
    {
        if(set_is_member(set1,(const void*)tmp)==0)
        {
            return -1;
        }
        tmp=tmp->next;
    }
    return 1;
}

//集合覆盖问题，已知一个S集合，已经它的n个子集
//选出最少的子集数，该子集集能包含S集合的所有元素
/*
原理：使用贪心算法，不断找出子集中能包含S中元素最多的子集
*/
//set存储找到的元素的集合，set1为全集合，subset为子集合
//set存放已经被找到的元素，该集合需要之前被初始化
int find_max_set(const Set* set,const Set *set1,const Set *subset)
{
    SetEle *element=subset->head;
    int count=0;
    while(element)
    {
        if(set_is_member(set1,(const void*)element->data))
        {   
            if(set_size(set)!=0)
            {
                if(set_is_member(set,(const void*)element->data)==0)
                    count++;
            }
            else
            {
                count++;
            }
        }
        element=element->next;
    }
    return count;
}

/**
 * 先找到最大的子集，然后对将该子集放入set中，
*/
int set_max_set(Set *set,const Set *set1,const Set *subset)
{
    SetEle *element=subset->head;
    while(element)
    {
        if(set_is_member(set1,(const void*)element->data))//该元素在S集合中
        {
            
            if(set_size(set)==0)//set集合是空集
            {
                set->head=element;
                set->tail=element;
            }
            else
            {
                if(set_is_member(set,(const void*)element->data))//该元素在set集合中就跳过
                {
                    element=element->next;
                    continue;
                } 
                set->tail->next=element;
                set->tail=element;
            }
            set->size++;
        }
        element=element->next;
    }
    return 0;
}


int main(int argc,char *argv[])
{
    Set set[6];
    SetEle *p;
    int arr[][10]={{0,1,5,2,4,3,7,9,8,6},{6,7,8,9},{3,4,5,6,7},{0,1,2},{8,9}};
    int len[]={10,4,5,3,2};
    int max_len=-1,count,max_order;
    for(int i=0;i<6;i++)
        set_init(set+i,match,NULL);
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<len[i];j++)
            set_insert(set+i+1,arr[i][j]);
    }
    int a[10],k=0,flag=0;
    while(set_size(set)<set_size(set+1))
    {
        for(int i=2;i<5;i++)
        {
            flag=0;
            for(int j=0;j<k;j++)
            {
                if(a[j]==i)
                {
                    flag=1;
                    break;
                } 
            }
            if(flag) continue;
            count=find_max_set(set,set+1,set+i);
            if(count>max_len){
                max_len=count;
                max_order=i;
            }
        }
        a[k++]=max_order;
        set_max_set(set,set+1,set+max_order);
        max_len=-1;
    }
    
    p=set[0].head;
    for (int i=0;i<set_size(set);i++)
    {
        printf("%d ",(int)p->data);
        p=p->next;
    }
 
}
