#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#include "struct.h"
#include "generator.h"
#include "error.h" 

void showList(tListOfInstr *L)
{
       
        L->active=L->first;
 
        while(L->active !=NULL)
        {      
 
                printf("\nInstrukcia :" );
                switch(L->active->Instr->Iname)
                {
                       
       
       
                        case 0:
                        printf("I_LABEL\n");
                        break;
                       
                        case 1:
 
                        printf("I_RETURN\n");                  
                        break;
                       
                        case 2:
 
                        printf("I_CALL\n");
                        break;
                       
                        case 3:
 
                        printf("I_GOTO\n");
                        break;
                       
                        case 4:
 
                        printf("I_ADD\n");
                        break;
                       
                        case 5:
 
                        printf("I_SUB\n");
                        break;
                       
                        case 6:
 
                        printf("I_MUL\n");
                        break;
                       
                        case 7:
 
                        printf("I_DIV\n");
                        break;
                       
                        case 8:
 
                        printf("I_MOD\n");
                        break;
                       
                        case 9:
 
                        printf("I_LESS_CMP\n");
                        break;
                       
                        case 10:
                        printf("I_GR_CMP\n");
                        break;
 
                        case 11:
                        printf("I_LESS_EQUAL_CMP\n");
                        break;
 
                        case 12:
                        printf("I_GR_EQUAL_CMP\n");
                        break;
 
                        case 13:
                        printf("I_EQUAL_CMP\n");
                        break;
 
                        case 14:
                        printf("I_NOT_EQUAL_CMP\n");
                        break;
 
                        case 15:
                        printf("I_ASSIGN\n");
                        break;
 
                        case 16:
                        printf("I_PRINT\n");
                        break;
 
                        case 17:
                        printf("I_READ\n");
                        break;
 
                        case 18:
                        printf("I_PUSH\n");
                        break;
 
                        case 19:
                        printf("I_POP\n");
                        break;
 
                        case 20:
                        printf("I_COPY\n");
                        break;
 
                        case 21:
                        printf("I_FIND\n");
                        break;
 
                        case 22:
                        printf("I_LENGTH\n");
                        break;
 
                        case 23:
                        printf("I_SORT\n");
                        break;
                        
                        default:
                        break;
                   }
                printf("A:");
                //printf("%d\n",active->Instr->a->type );
                if (L->active->Instr->a!=NULL)
                {
					switch(L->active->Instr->a->type)
					{
	 
							case tINTEGER:
									printf("tINTEGER: ");
									break;
							case tSTRING:
									printf("tSTRING: ");
									break;
							case tREAL:
									printf("tREAL: ");
									break;
							case tBOOLEAN:
									printf("tBOOLEAN: ");
									break;
							case tVAR:
									printf("tVAR: ");
									break;
									default:
									break;
					}
				} 
				if (L->active->Instr->a!=NULL)
                {
					switch(L->active->Instr->a->type)
					{
							break;
							case tSTRING:
									printf("%s\n",L->active->Instr->a->data.s);
									break;
							case tINTEGER:
									printf("%d\n",L->active->Instr->a->data.i);
									break;
							case tREAL:
									printf("%f\n",L->active->Instr->a->data.r);
									break;
							case tBOOLEAN:
									printf("%d\n",L->active->Instr->a->data.b);
									break;
							case tVAR:
									printf("%s\n",L->active->Instr->a->data.s);
									break;
	 
									default:
									break;
					}
				}
				else printf("NULL\n");
 
 
                printf("B:");
                //printf("%d\n",active->Instr->a->type );
                if (L->active->Instr->b!=NULL)
                {
					switch(L->active->Instr->b->type)
					{
	 
							case tINTEGER:
									printf("tINTEGER: ");
									break;
							case tSTRING:
									printf("tSTRING: ");
									break;
							case tREAL:
									printf("tREAL: ");
									break;
							case tBOOLEAN:
									printf("tBOOLEAN: ");
									break;
							case tVAR:
									printf("tVAR: ");
									break;
									default:
									break;
					}
				}
				if (L->active->Instr->b!=NULL)
                {
					switch(L->active->Instr->b->type)
					{
							break;
							case tSTRING:
									printf("%s\n",L->active->Instr->b->data.s);
									break;
							case tINTEGER:
									printf("%d\n",L->active->Instr->b->data.i);
									break;
							case tREAL:
									printf("%f\n",L->active->Instr->b->data.r);
									break;
							case tBOOLEAN:
									printf("%d\n",L->active->Instr->b->data.b);
									break;
							case tVAR:
									printf("%s\n",L->active->Instr->b->data.s);
									break;
	 
									default:
									break;
					}
				}
				else printf("NULL\n");
				
				if (L->active->Instr->res!=NULL)
                {
					printf("RES: %s\n", L->active->Instr->res);
				}
				else printf("RES: NULL\n");
               
                L->active= L->active->nextItem;
 
        }
        return;
}

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
		if(L->first->Instr->a->data.s != NULL)		//zakomentovane pre interpret, hadzalo errory
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
	if ((L->first->Instr->res)!=NULL)
    {
		free(L->first->Instr->res);
	}
	
    L->first = L->first->nextItem;
    free(tmp);
    free(instr);
  }
}

tListItem* generator(tListOfInstr *L, InstName name, Variable *a, Variable *b, char *s)
{
	tInstruction *instr=malloc(sizeof(tInstruction));
	
	instr->Iname=name;
	instr->a=a;
	instr->b=b;
	instr->res=NULL;
	if(s!=NULL)
	{
		instr->res=malloc(strlen(s)+1);
		if (instr->res==NULL) printf("je to napicu\n");
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
