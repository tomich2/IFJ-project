#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//zasobnik pracuje s pamatou akejkolvek velkosti
//polozka zasobnika
typedef struct stack_item S_item;
struct stack_item
{
 void *data;
 S_item * previous;
}; 

//struktura zasobika - it_size -velkost jednej polozky
//pri pushovani mozne velkost prisposobit polozke pomocou paramtera size_sp
typedef struct stack_s 
{
 size_t it_size;
 S_item *Top;	 
} Stack;

void init(Stack *stack, size_t i_size);
void push(Stack *stack, void *push_data, int size_sp);
int pop(Stack *stack);
void *top(Stack *stack);
bool S_empty(Stack *stack);
void S_erase(Stack *stack);
#endif
