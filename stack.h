#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct stack_item S_item;
struct stack_item
{
 char *data;
 S_item * previous;
}; 

typedef struct stack_s 
{
 S_item *Top;	 
} Stack;

void init(Stack *stack);
void push(Stack *stack, char *push_data);
int pop(Stack *stack);
char *top(Stack *stack);
bool S_empty(Stack *stack);

#endif
