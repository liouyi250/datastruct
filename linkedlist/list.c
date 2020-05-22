#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"


void list_init(List *list,void (*destroy)(void *data))
{
	list->head=NULL;
	list->tail=NULL;
	list->size=0;
	list->destroy=destroy;
}

void list_destroy(List *list)
{
	void *data;
	while(list_size(list)>0){
		if(list_rem_next(list,NULL,&data)==0 && list->destroy!=NULL){
			list->destroy(data);
		}
	}
	memset(list,0,sizeof(List));
}

int list_ins_next(List *list,struct element *ele,const void* data)
{
	struct element *insertEle=(struct element*)malloc(sizeof(struct element));
	if(insertEle==NULL) return -1;
	insertEle->data=(void*)data;
	insertEle->next=NULL;

	if(list->size==0)
	{
		list->head=insertEle;
		list->tail=insertEle->next;
		list->size++;
		return 0;
	}
	else{
		if(ele==NULL)
		{
			insertEle->next=list->head;
			list->head=insertEle;
			list->size++;
			return 0;
		}else{
			insertEle->next=ele->next;
			ele->next=insertEle;
			list->size++;
			return 0;
		}
	}
	return -1;
}

int list_rem_next(List *list,struct element *ele,void **data)
{
	struct element* old_element;
	if(list_size(list)==0) return -1;
	if(ele==NULL){
		*data=list->head->data;
		old_element=list->head;
		list->head=list->head->next;

		if(list_size(list)==1) list->tail=NULL;
	}else{
		if(ele->next==NULL) return -1;
		*data=ele->next->data;
		old_element=ele->next->next;
		if(ele->next==NULL) list->tail=ele;
	}
	free(old_element);
	list->size--;
	return 0;
}


