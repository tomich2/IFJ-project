// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>

typedef enum{NONTERMINAL, TERMINAL, EMPTY=-1}ItemType;
typedef union{
        int nonterm;
        identita term;
        }U_Item;

typedef struct{
        ItemType type;
        U_Item value;
        }T_ParserItem;


int top_down();
void PItems_alloc(T_ParserItem ***Ptr);
void PItems_free(T_ParserItem ***Ptr);



#endif
