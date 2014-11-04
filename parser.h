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

typedef enum{NONTERMINAL, TERMINAL}ItemType;
typedef union{
        int nonterm;
        TOKEN *term;
        }U_Item;
typedef struct{
        ItemType type;
        U_Item value;
        }T_ParserItem;

size_t PItem_size=sizeof(T_ParserItem*);
#endif
