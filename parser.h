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
#include "stack.h"
#include "ial.h"

#define GTAB_SIZE 11

typedef struct non_term
{
	int type;
	void *data;
} nont;

typedef enum{NONTERMINAL, TERMINAL, EMPTY=-1}ItemType;

typedef enum{GLOBVAR_DEK, FUNC_DEF, LOCVAR_DEK, MAIN_P}T_State;

typedef union{
        nont nonterm;
        identita term;
        }U_Item;

typedef struct{
        ItemType type;
        U_Item value;
        }T_ParserItem;

typedef struct{
        int def; // 0-nedefinovana, 1-definovana
        char *ret_par_types; // retazec zaciatocnych pismen typov navratovej hodnoty a parametrov
        }T_FuncData;

ERROR_MSG top_down();
void PItems_alloc(T_ParserItem ***Ptr);
void PItems_free(T_ParserItem ***Ptr);
void free_all(T_ParserItem **p, Stack st, int stack_erase, htab_t *gsymtab);


#endif
