// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexical.h"
#include "lexstring.h"
#include "error.h"
#include "garbage.h"

void InitList (TGarbage *L)
{
    L->Act = NULL;
    L->First = NULL;
}

void InsertFirst (TGarbage * L, char * Elem)
{
    TGarbagePtr p=L->First;
    L->First=malloc(sizeof(struct TGarbage));
    if (L->First==NULL)
    {
        return;
    }
    L->First->data=Elem;
    L->First->ptr=p;
}

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
