// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexical.h"
#include "lexstring.h"
#include "error.h"

int kluc=1;

char *string_implementation (char c,int i,char *mem)
{
    if (i%(NATIVE_CHAR-1)==0 && i>0) mem=allocate_string(i,mem);                                // ak je potreba viac miesta, alokuj
    mem[i]=c;                                                                                   // uloz znak
    return mem;                                                                                 // vrat ukazovatel
}

char *allocate_string (int i,char *mem)
{
    char *mem_realloc;

    if (i%(NATIVE_CHAR-1)==0 && i>0)                                                            // ak nemas miesto alokuj
    {
        mem_realloc=(char*)realloc(((char*)mem),(sizeof(char)*NATIVE_CHAR*kluc));               // uvolni pomocou realloc tolko miesta kolko potrebujes
        if (mem_realloc==NULL)
        {
            close_file();
            Error(99);
        }
        kluc++;                                                                                 // inkrementuj konstantu velkosti alokovanej pamate
        mem=mem_realloc;
    }
    return mem;
}

char *first_allocation ()
{
    token->mem=(char*) malloc(sizeof(char)*NATIVE_CHAR);                                        // alokuj prvy raz
    kluc=2;
    if (token->mem==NULL)
    {
        close_file();
        Error(99);
    }
    return token->mem;
}
