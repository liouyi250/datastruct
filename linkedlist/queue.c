#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

int queue_enqueue(Queue *queue,const void *data)
{
    QueueEle *element =(QueueEle*)malloc(sizeof(QueueEle));
    if(element==NULL) return -1;
    element->data=(void*)data;
    if(queue_size(queue)==0)
    {
        queue->head=element;
        queue->tail=element;
    }
    else
    {
        queue->tail->next=element;
        queue->tail=element;
    }
    element->next=NULL;
    queue->size++;
    return 0;
}
int queue_dequeue(Queue *queue,void **data)
{
    if(queue_peek(queue)==NULL) return -1;
    QueueEle *element=queue->head;
    *data=element->data;
    queue->head=element->next;
    free(element);
    element=NULL;
    queue->size--;
    return 0;
}

int main(int argc,char *argv[])
{
    char str[]="Welcome";
    Queue queue;
    void *data;
    queue_init(&queue,NULL);
    queue_enqueue(&queue,(const void*)str[0]);
    queue_enqueue(&queue,(const void*)str[1]);
    queue_dequeue(&queue,&data);
    printf("%c\n",(char)data);
    queue_dequeue(&queue,&data);
    printf("%c\n",(char)data);
    queue_dequeue(&queue,&data);
    char *p=str;
    while(*p!='\0')
    {
        queue_enqueue(&queue,(const void*)*p);
        p++;
    }
    while(queue_dequeue(&queue,&data)!=-1)
    {
        printf("%c ",(char)data);
    }
    return 0;
}