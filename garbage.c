// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //

/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexical.h"
#include "lexstring.h"
#include "error.h"
#include "garbage.h"

// Subor garbage.c obsahuje definiciu funkcii potrebnych pre pracu s tzv. Garbage collectorom,
// tento collector bol pouzity z dovodu, ze pri volani funkcii je potrebne dynamicky pracovat
// s pamatou a nasledne ju aj transparentne uvolnit

// Inicializacia zoznamu pre garbage collector
void InitList (TGarbage *L)
{
    L->Act = NULL;
    L->First = NULL;
}

// Vlozi novy prvok zoznamu, najskor prehlada, ci sa tam rovnaky prvok uz nenachadza,
// aby nedoslo k dvojnasobnemu uvolneniu
void InsertFirst (TGarbage * L, char * Elem)
{
    TGarbagePtr uk=L->First;
    while (uk!=NULL)
    {
        if (uk->data==Elem) return;
        uk = uk->ptr;
    }
    TGarbagePtr p=L->First;
    L->First=malloc(sizeof(struct TGarbage));
    if (L->First==NULL)
    {
        return;
    }
    L->First->data=Elem;
    L->First->ptr=p;
}

// Uvolni cely zoznam aj vsetky naalokovane ukazovatele
void DisposeList (TGarbage * L)
{
    TGarbagePtr p=NULL;

    while (L->First != NULL)
    {
        p=L->First;
        L->First=L->First->ptr;
        free((p->data));
        free (p);
    }

    L->Act=NULL;
}
