#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "struct.h"
#include "generator.h"
#include "error.h" 

void listInit(tListOfInstr *L)
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}
  
void listFree(tListOfInstr *L)
{
  tListItem *tmp;
  tInstruction *instr;
  struct Variable *var;
  while (L->first != NULL)
  {
    tmp = L->first;
    instr= L->first->Instr;
    if ((L->first->Instr->a)!=NULL)
    {
		if(L->first->Instr->a->data.s != NULL)
			free(L->first->Instr->a->data.s);
		var = L->first->Instr->a;
		free(var);
	}
	if ((L->first->Instr->b)!=NULL)
    {
		if(L->first->Instr->b->data.s != NULL)
			free(L->first->Instr->b->data.s);
		var = L->first->Instr->b;
		free(var);
	}
	
    L->first = L->first->nextItem;
    free(tmp);
    free(instr);
  }
}

tListItem* generator(tListOfInstr *L, InstName name, Variable *a, Variable *b, char *s)
{
	tInstruction *instr=malloc(sizeof(tInstruction));
	//if (instr==NULL)
	//	Error(7);
	instr->Iname=name;
	instr->a=a;
	instr->b=b;
	instr->res=s;
	
	tListItem *newItem;
	newItem = malloc(sizeof (tListItem));
//	if (newItem==NULL)
	//	Error(7);
	newItem->Instr = instr;
	newItem->nextItem = NULL;
	if (L->first == NULL)
		L->first = newItem;
	else
		L->last->nextItem=newItem;
	L->last=newItem;
  
	return newItem;
}
