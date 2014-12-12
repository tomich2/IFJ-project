// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //

/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak

#ifndef _GARBAGE_H
#define _GARBAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Subor garbage.h obsahuje deklaraciu funkcii a struktur potrebnych pre pracu s tzv. Garbage collectorom,
// tento collector bol pouzity z dovodu, ze pri volani funkcii je potrebne dynamicky pracovat
// s pamatou a nasledne ju aj transparentne uvolnit

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
