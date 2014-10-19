// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef _LEXSTRING_H_
#define _LEXSTRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

char *string_implementation (char c,int i,char *mem);               // naalokuje zakladnu velkost pola znakov
char *allocate_string(int i,char *mem);                             // alokuj string
char *first_allocation ();                                          // vrat ukazovatel na alokovanu oblast

#endif
