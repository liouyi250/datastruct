#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

int stack_push(Stack *stack,const void *data)
{
    StackEle *element=(StackEle*)malloc(sizeof(StackEle));
    if(element==NULL) return -1;
    element->data=(void*)data;
    if(stack_peek(stack))
    {
        element->next=stack->head;
        stack->head=element;
    }
    else
    {
        stack->head=element;
        element->next=NULL;
    }
    stack->size++;
    return 0;
}
int stack_pop(Stack *stack,void **data)
{
    if(stack_peek(stack)==NULL) return -1;
    StackEle *element=stack->head;
    *data=element->data;
    stack->head=element->next;
    stack->size--;
    return 0;
}

int main(int argc,char *argv[])
{
    Stack stack;
    char *str[]={"Hello","new","world","human"};
    void *data;
    stack_init(&stack,NULL);
    stack_push(&stack,(const void*)str[0]);
    stack_push(&stack,(const void*)str[1]);
    stack_pop(&stack,&data);
    printf("%s\n",(char*)data);
    stack_pop(&stack,&data);
    printf("%s\n",(char*)data);
    stack_pop(&stack,&data);
    stack_push(&stack,(const void*)str[2]);
    stack_pop(&stack,&data);
    printf("%s\n",(char*)data);
    stack_pop(&stack,&data);
}