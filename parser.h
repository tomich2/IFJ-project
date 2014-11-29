// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"
#include "ial.h"

#define GTAB_SIZE 11

typedef struct non_term
{
	int type;
	void *data;
} nont;

typedef enum{NONTERMINAL, TERMINAL, EMPTY=-1}ItemType;

typedef enum{GLOBVAR_DEK, FUNC_ID, FUNC_PARAMS, FUNC_TYPE, LOCVAR_DEK, FUNC_BODY, MAIN_BODY}T_State;

typedef enum{tINTEGER, tSTRING, tREAL, tBOOLEAN}T_vartype;

typedef union{
        nont nonterm;
        identita term;
        }U_Item;

typedef struct{
        ItemType type;
        U_Item value;
        }T_ParserItem;

typedef struct{
        T_vartype type;
        bool is_def;
        }T_VarData;

typedef struct{
        bool is_def;
        char *ret_par_types; // retazec zaciatocnych pismen typov navratovej hodnoty a parametrov
        }T_FuncData;

typedef struct{
        char *act_varID;
        char *act_funcID;
        int n;
        char *act_rptypes;
        }T_Actual;

ERROR_MSG top_down();
ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac);
ERROR_MSG PItems_alloc(T_ParserItem ***Ptr);
void PItems_free(T_ParserItem ***Ptr);
void free_all(T_ParserItem **p, Stack st, int stack_erase, int token_mem_free, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac);


#endif

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
