#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdlib.h>
#include "struct.h"


typedef struct listItem
{
  tInstruction *Instr;
  struct listItem *nextItem;
} tListItem;

typedef struct tListOfInstr
{
  tListItem *first;  
  tListItem *last;   
  tListItem *active;
} tListOfInstr;


void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
void listInsertLast(tListOfInstr *L, tInstruction *I);
tInstruction* generator(InstName name, Variable *a, Variable *b, void *s);



#endif
