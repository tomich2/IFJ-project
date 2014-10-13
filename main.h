// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define NATIVE_CHAR 10



FILE *fp;
int c;
int k=1;
typedef struct token
{
    int identity;
    char *mem;
} TOKEN;

//retazec klucovych slov

const char key_words [] = {",begin,boolean,do,else,end,false,find,forward,function,if,integer,readln,real,sort,string,then,true,var,while,write,"};

// identita oznacuje typ tokenu
// 1 - identifikator
// 2 - integer
// 3 - real
// 4 - boolean
// 5 - string
// 6 - klucove slovo
// 7 - operator

TOKEN *token;

TOKEN *get_token ();
int open_file (char *filename,int argc);
void close_file ();
void instant_comment ();
char *string_implementation (char c,int i,char *mem);
char *allocate_string(int i,char *mem);
char *first_allocation ();
int is_key_word (char *key);
