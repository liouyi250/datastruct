#include <stdio.h>
#include <stdlib.h>
#include "clist.h"

void clist_init(CList *list,void (*destroy)(void *data))
{
    list->size=0;
    list->head=NULL;
    list->destroy=destroy;
}
void clist_destroy(CList *list)
{
    void *data;
    while(clist_size(list)>0)
    {
        if(clist_rem_next(list,list->head,&data)==0 && list->destroy!=NULL)
            list->destroy(data);
    }
    
}
int clist_ins_next(CList *list,CListEle *element,const void *data)
{
    CListEle *ele=(CListEle*)malloc(sizeof(CListEle));
    if(ele==NULL) return -1;
    ele->data=(void*)data;
    if(element==NULL)
    {
        list->head=ele;
        ele->next=list->head;
    }
    else
    {
        ele->next=element->next;
        element->next=ele;
        
    }
    list->size++;
    return 0;
}
int clist_rem_next(CList *list,CListEle *element,void **data)
{
    CListEle *ele=(CListEle*)element->next;
    *data=ele->data;
    if(ele==ele->next)
    {
        list->head=NULL;
    }
    else
    {
        element->next=ele->next;
    }
    free(ele);
    ele=NULL;
    list->size--;
    return 0;
}


int main(int argc,char* argv[])
{
    int a[]={1,3,5,7};
    char *str[]={"abc","DEF"};
    void *data;
    CList list;
    
    clist_init(&list,NULL);
    clist_ins_next(&list,NULL,(const void*)a[0]);
    clist_ins_next(&list,list.head,(const void*)str[1]);
    clist_ins_next(&list,list.head,(const void*)a[2]);
    clist_ins_next(&list,list.head,(const void*)str[0]);
    clist_rem_next(&list,list.head,&data);
    printf("%s\n",(char*)data);
    clist_destroy(&list);
    CListEle *pEle=list.head;
    for(int i=0;i<list.size;i++){
        if(i<2)
            printf("%d\n",clist_data(pEle));
        else
        {
            printf("%s\n",clist_data(pEle));
        }
        
        pEle=pEle->next;
    }
}