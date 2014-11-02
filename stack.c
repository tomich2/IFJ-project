#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"

//inicializuje zasobnik
void init(Stack *stack, size_t i_size)
{
 stack->it_size=i_size;
 stack->Top=NULL;
 return;
}

//push - prida prvok do zasobniku, vypise chybu ak sa nepodari alokovat pamat
//param size_sp - velkost pushovanej polozky - ak sa zada nieco mensie ako 0 
//pouzije sa vseobecna velkost ulozena v stack->it_size
void push(Stack *stack, void *push_data, int size_sp)
{
 S_item *tmp;
 size_t size=stack->it_size;
 if (size_sp>0) size=size_sp+1;
 tmp=(S_item *) malloc(sizeof(S_item));
 if (tmp==NULL) 
 	{
	printf("Malloc error!\n");
	return;
	}

 tmp->data=(void *) malloc(size);
 if (tmp->data==NULL) 
 	{
	printf("Malloc error!\n");
	return;
	}
 memcpy(tmp->data,push_data,(size));
 tmp->previous=stack->Top;
 stack->Top=tmp;
 return;
}

//funkcia pop - uvolni posledny prvok zasobnika, ak je zasobnik prazdny vrati -1
int pop(Stack *stack)
{
 if (S_empty(stack)==true) return -1;
 S_item *tmp=stack->Top;
 stack->Top=tmp->previous;
 free(tmp->data);
 free(tmp);
 return 0;
}

//vrati true ak je zasobnik prazdny
bool S_empty(Stack *stack)
{
 return (stack->Top==NULL) ? true:false;
}

//vrati ukazovatel na string na poslednom mieste zasobnika
void *top(Stack *stack)
{
 S_item *tmp=stack->Top;
 return tmp->data;
}


