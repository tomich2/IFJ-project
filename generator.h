// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <stdlib.h>
#include "struct.h"

/*zoznam instrukcii*/
typedef struct listItem
{
  tInstruction *Instr;
  struct listItem *nextItem;
} tListItem;

typedef struct tListOfInstr
{
  tListItem *first;  // ukazatel na prvni prvek
  tListItem *last;   
  tListItem *active;	//ukazatel na aktivny prvek
} tListOfInstr;

void showList(tListOfInstr *L);
void listInit(tListOfInstr *L);
void listFree(tListOfInstr *L);
tListItem* generator(tListOfInstr *L, InstName name, Variable *a, Variable *b, char *s);



#endif
