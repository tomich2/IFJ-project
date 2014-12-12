// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //

/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexical.h"
#include "lexstring.h"
#include "error.h"

//
// Subor lexstring.c obsahuje implementaciu funkcii potrebnych pre funkciu get_token(), hlavne
// kvoli dynamickej praci s pamatou
//

int kluc=1;                                                                                     // pomocna premenna

// Funkcia string_implementation sluzi na zistenie, ci je potrebne naalokovate viac miesta
// alebo je velkost pola znakov dostacujuca.
// Sluzi na ulozenie jedneho znaku do pola

char *string_implementation (char c,int i,char *mem)
{
    if (i%(NATIVE_CHAR-1)==0 && i>0) mem=allocate_string(i,mem);                                // ak je potreba viac miesta, alokuj
    if (mem==NULL) return NULL;
    mem[i]=c;                                                                                   // uloz znak
    return mem;                                                                                 // vrat ukazovatel
}

// Funkcia aloocate_string naalokuje miesto potrebne pre ulozenie retazca

char *allocate_string (int i,char *mem)
{
    char *mem_realloc;

    if (i%(NATIVE_CHAR-1)==0 && i>0)                                                            // ak nemas miesto alokuj
    {
        mem_realloc=(char*)realloc(((char*)mem),(sizeof(char)*NATIVE_CHAR*kluc));               // uvolni pomocou realloc tolko miesta kolko potrebujes
        if (mem_realloc==NULL)
        {
            return NULL;
        }
        kluc++;                                                                                 // inkrementuj konstantu velkosti alokovanej pamate
        mem=mem_realloc;
    }
    return mem;
}

// Funkcia first_allocation sa vola pri prvej alokacii retazca

char *first_allocation ()
{
    token->mem=(char*) malloc(sizeof(char)*NATIVE_CHAR);                                        // alokuj prvy raz
    kluc=2;
    if (token->mem==NULL)
    {
        return NULL;
    }
    return token->mem;
}
