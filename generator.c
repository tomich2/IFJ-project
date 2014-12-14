// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "struct.h"
#include "generator.h"
#include "error.h" 


/*inicializacia listu*/
void listInit(tListOfInstr *L)
{
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
}
  
/*uvolnenie listu instrukcii*/
void listFree(tListOfInstr *L)
{
  tListItem *tmp;
  tInstruction *instr;
  struct Variable *var;
  while (L->first != NULL)
  {
    tmp = L->first;
    instr= L->first->Instr;
    if (L->first->Instr->a != NULL)
    {
		if(L->first->Instr->a->data.s != NULL)		
			free(L->first->Instr->a->data.s);
		var = L->first->Instr->a;
		free(var);
	}
	if (L->first->Instr->b != NULL)
    {
		if(L->first->Instr->b->data.s != NULL)
			free(L->first->Instr->b->data.s);
		var = L->first->Instr->b;
		free(var);
	}
	if ((L->first->Instr->res)!=NULL)
    {
		free(L->first->Instr->res);
	}
	
    L->first = L->first->nextItem;
    free(tmp);
    free(instr);
  }
}

/*vygeneruje instrukciu s danymi operandami a vlozi ju do zoznamu*/
tListItem* generator(tListOfInstr *L, InstName name, Variable *a, Variable *b, char *s)
{
	tInstruction *instr=malloc(sizeof(tInstruction));
	
	instr->Iname=name;
	instr->a=a;
	if(a !=NULL)
		if(a->type != tSTRING)
			if(a->type != tVAR)
				a->data.s=NULL;
	instr->b=b;
	if(b!=NULL)
		if(b->type != tSTRING)
			if(b->type != tVAR)
				b->data.s=NULL;
	instr->res=NULL;
	if(s!=NULL)
	{
		instr->res=malloc(strlen(s)+1);
		strcpy(instr->res,s);
	}
	
	tListItem *newItem;
	newItem = malloc(sizeof (tListItem));

	newItem->Instr = instr;
	newItem->nextItem = NULL;
	if (L->first == NULL)
		L->first = newItem;
	else
		L->last->nextItem=newItem;
	L->last=newItem;
  
	return newItem;
}
