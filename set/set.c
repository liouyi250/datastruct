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
    while(tmp)
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

int main(int argc,char *argv[])
{
    Set set1,set2,setu,setd;
    SetEle *p;
    set_init(&set1,match,NULL);
    set_init(&set2,match,NULL);
    int arr1[]={0,1,5,2,3,3};
    int arr2[]={2,2,3,3,4,4};
    int *data=1;
    for(int i=0;i<6;i++)
        set_insert(&set1,(const void*)arr1[i]);
    for(int i=0;i<6;i++)
        set_insert(&set2,(const void*)arr2[i]);
    set_remove(&set1,&data);
    set_remove(&set1,&data);
    set_union(&setu,&set1,&set2);
    set_difference(&setd,&set1,&set2);
    printf("%d\n",set_size(&set1));
    printf("%d\n",set_size(&setu));
    p=setd.head;
    while (p)
    {
        printf("%d ",(int)p->data);
        p=p->next;
    }  
}
