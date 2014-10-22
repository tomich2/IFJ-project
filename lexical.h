// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef _LEXICAL_H_
#define _LEXICAL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define NATIVE_CHAR 10                                              //velkost retazca pre ukladanie

// identita oznacuje typ tokenu
// 1 - identifikator
// 2 - integer
// 3 - real
// 4 - boolean
// 5 - string
// 6 - klucove slovo
// 7 - operator

typedef enum TYP {identifikator = 1, integer, real, boolean, string, kluc_slovo, operator} identita;


typedef struct token
{
    identita identity;                                              // prislusnost ku skupine
    char *mem;                                                      // ukazovatel na retazec
} TOKEN;



TOKEN *token;                                                       // struktura tokenu
FILE *fp;                                                           //subor
int c;                                                              //nacitavany znak

TOKEN *get_token ();                                                // v ukazovateli vrat jeden token
int open_file (char *filename,int argc);                            // otvor subor
void close_file ();                                                 // zavri subor
int is_key_word (char *key);                                        // zisti ci ide o klucove slovo

#endif
