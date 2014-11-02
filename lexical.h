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
// identifikator
// keywords: begin, boolean, do, else, end
// boolean: false
// keywords: find, forward, function, if, integer, readln, real,
//           sort, string, then
// boolean: true
// keywords: var, while, write
// operatory: +,-,*,/,:,:=,;,>,<,>=,<=,=,(,),.
// udajovy typ: integer, char, real, boolean, string
// operatory: <>, ciarka - ,

typedef enum TYP {ID = 1, KwBegin = 2, KwBoolean = 3, KwDo = 4, KwElse = 5, KwEnd = 6, BooleanFalse = 7, KwFind = 8,
                  KwForward = 9, KwFunction = 10, KwIf = 11, KwInteger = 12, KwReadln = 13, KwReal = 14,
                  KwSort = 15, KwString = 16, KwThen = 17, BooleanTrue = 18, KwVar = 19, KwWhile = 20,
                  KwWrite = 21, OpPlus = 22, OpMinus = 23, OpKrat = 24, OpDiv = 25, OpDek = 26,
                  OpPrir = 27, OpKonec = 28, OpVacsi = 29, OpMensi = 30, OpVacsiR = 31, OpMensiR = 32,
                  OpRovny = 33, OpLZat = 34, OpPZat = 35, OpBodka = 36, DtInteger = 37, DtChar = 38,
                  DtReal = 39, DtBoolean = 40, DtString = 41, OpNerovny = 42, OpCiarka = 43} identita;


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
int CaseInsensitiveCharCmp (char c1, char c2);                      // porovna dva znaky na case insensitive
#endif
