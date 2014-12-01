#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "struct.h"
#include "generator.h"
#include "error.h" 
#include "error.c"

void listInit(tListOfInstr *L)
{
  L->first  = NULL;
  L->last   = NULL;
}
  
void listFree(tListOfInstr *L)
{
  tListItem *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    free(ptr);
  }
}

void listInsertLast(tListOfInstr *L, tInstruction *I)
{
  tListItem *newItem;
  newItem = malloc(sizeof (tListItem));
  if (newItem==NULL)
	Error(7);
  newItem->Instr = I;
  newItem->nextItem = NULL;
  if (L->first == NULL)
     L->first = newItem;
  else
     L->last->nextItem=newItem;
  L->last=newItem;
}


tInstruction* generator(InstName name, Variable *a, Variable *b, void *s)
{
	tInstruction *instr=malloc(sizeof(tInstruction));
	if (instr==NULL)
		Error(7);
	instr->Iname=name;
	instr->a=a;
	instr->b=b;
	instr->res=s;
	return instr;
}
