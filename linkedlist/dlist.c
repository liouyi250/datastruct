#include <stdlib.h>
#include <stdio.h>
#include "dlist.h"

void dlist_init(DList *list,void (*destroy)(void *data)){
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
    list->destroy=destroy;
}

void dlist_destroy(DList *list)
{
    void *data;
    while(dlist_size(list)>0)
    {
        if(dlist_remove(&list,NULL,&data)==0 && list->destroy!=NULL)
        {
            list->destroy(data);
        }
    }
}

void dlist_empty_ins(DList *list,DListEle *insertEle)
{
    list->head=insertEle;
    list->tail=insertEle;
    insertEle->next=NULL;
    insertEle->prev=NULL;
}

int dlist_ins_next(DList *list,DListEle *element,const void *data)
{
    DListEle *insertEle=(DListEle*)malloc(sizeof(DListEle));
    if(insertEle==NULL) return -1;
    insertEle->data=(void*)data;
    if(element==NULL)
    {
        if(dlist_size(list)==0)
        {
            dlist_empty_ins(list,insertEle);
        }
        else
        {
            insertEle->prev=list->tail;
            insertEle->prev->next=insertEle;
            list->tail=insertEle;
            insertEle->next=NULL;
        }
    }
    else
    {
        insertEle->prev=element;
        insertEle->next=element->next;
        if(element->next!=NULL)
        {
            element->next->prev=insertEle;
        } 
        element->next=insertEle;
    }
    list->size++;
    return 0;
    
}

int dlist_ins_prev(DList *list,DListEle *element,const void *data)
{
    DListEle *insertEle=(DListEle*)malloc(sizeof(DListEle));
    if(insertEle==NULL) return -1;
    insertEle->data=(void*)data;
    if(element==NULL){
        if(dlist_size(list)==0){
            dlist_empty_ins(list,insertEle);
        }else
        {
            insertEle->next=list->head;
            list->head->prev=insertEle;
            list->head=insertEle;
            insertEle->prev=NULL;
        }
    }else{
        insertEle->next=element;
        insertEle->prev=element->prev;
        if(element->prev!=NULL)
        {
            element->prev->next=insertEle;
            
        }
        element->prev=insertEle;
    }
    list->size++;
    return 0;
}

int dlist_remove(DList *list,DListEle *element,void **data)
{
    if(element==NULL)
    {//remove the last element of the list
        DListEle *ele;
        ele=list->tail;
        *data=ele->data;
        if(list->head != list->tail)
        {
            list->tail=ele->prev;
            list->tail->next=NULL;
        }
        else
        {
            list->head=list->tail=NULL;
        }
        free(ele);
        ele=NULL;
    }
    else{
        *data=element->data;
        if(element->prev==NULL)
        {
            list->head=element->next;
            list->head->prev=NULL;
            
        }
        if(element->next==NULL)
        {
            element->prev->next=NULL;
            list->tail=element->prev;
            list->tail->next=NULL;
        }
        if(element->prev!=NULL && element->next!=NULL)
        {
            element->prev->next=element->next;
            element->next->prev=element->prev;
        }
        list->size--;
        element->prev=NULL;
        element->next=NULL;
        free(element);
        element=NULL;
    }
    list->size--;
    return 0;
}

int main(int argc,char *argv[])
{
    int a[]={1,2,3,4};
    int t;
    int *pt=&t;
    DList dlist;
    dlist_init(&dlist,NULL);
    dlist_ins_next(&dlist,NULL,a[0]);
    dlist_ins_next(&dlist,NULL,*(a+1));
    DListEle *ele1=dlist.head->next;
    dlist_ins_next(&dlist,NULL,*(a+2));
    
    dlist_ins_prev(&dlist,ele1,*(a+3));
    dlist_remove(&dlist,ele1,&pt);
    DListEle *ele=dlist.head;
    while(ele)
    {
        printf("%d\n",dlist_data(ele));
        ele=ele->next;
    }
}