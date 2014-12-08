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
  tListItem *first;  // ukazatel na prvni prvek
  tListItem *last;   // ukazatel na posledni prvek
  tListItem *active;
} tListOfInstr;

void showList(tListOfInstr *L);
void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
tListItem* generator(tListOfInstr *L, InstName name, Variable *a, Variable *b, char *s);



#endif
