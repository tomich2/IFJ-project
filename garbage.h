// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct TGarbage {
    struct TGarbage *ptr;
    char * data;
} *TGarbagePtr;


typedef struct {
    TGarbagePtr Act;
    TGarbagePtr First;
} TGarbage;

TGarbage *LGar;

void InitList (TGarbage * L);
void DisposeList (TGarbage * L);
void InsertFirst (TGarbage *, char * Elem);

#endif
