#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"

//inicializuje zasobnik
void init(Stack *stack)
{
 stack->Top=NULL;
 return;
}

//push - prida prvok do zasobniku, vypise chybu ak sa nepodari alokovat pamat
void push(Stack *stack, char *push_data)
{
 S_item *tmp;
 size_t size=strlen(push_data)+1;
 tmp=(S_item *) malloc(sizeof(S_item));
 if (tmp==NULL) 
 	{
	printf("Malloc error!\n");
	return;
	}

 tmp->data=(char *) malloc(size);
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
char *top(Stack *stack)
{
 S_item *tmp=stack->Top;
 return tmp->data;
}

//testovaci main
//najprv pushne s1 a vypise vrchol zasobnika a nasledne ho popne
//potom pushne s1 a s2 vypise vrchol popne a zase vypise vrchol a popne 
int main () 
{
 char s1[]="Ahoj";
 char s2[]="Svet";

 Stack stack;
 init(&stack);
 push(&stack,s1);
 printf("vrchol je %s\n", top(&stack));
 pop(&stack);
 if (S_empty(&stack)) printf("zasobnik je prazdny\n");
 push(&stack,s1);
 push(&stack,s2);
 printf("vrchol je %s\n", top(&stack));
 pop(&stack);
 printf("vrchol je %s\n", top(&stack));
 pop(&stack);
 if (S_empty(&stack)) printf("zasobnik je prazdny\n");
 return 0;
}
