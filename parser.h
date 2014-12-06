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
#include <stdbool.h>
#include "stack.h"
#include "ial.h"
#include "lexical.h"
#include "list.h"
#include "generator.h"

#define GTAB_SIZE 11
#define MAX_RPTYPES 20
#define TMPLEN 11 // dlzka unikatnej docasnej premennej
#define TMPU "42TMP14ifj"
#define TMPU2 "42TMP24ifj"
#define TMParam "42TMPar2ifj"

typedef enum{NONTERMINAL, TERMINAL, EMPTY=-1}ItemType;

typedef enum{GLOBVAR_DEK, FUNC_ID, FUNC_PARAMS, FUNC_TYPE, LOCVAR_DEK, FUNC_BODY, MAIN_BODY}T_State;

typedef struct non_term
{
	int type;
	T_vartype d_type;
	char *index;
} nont;

typedef struct tterm
{
	identita type;
	char *index;
} tterm;

typedef union{
        nont nonterm;
        tterm term;
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
        bool is_ret;
        char *ret_par_types; // retazec zaciatocnych pismen typov navratovej hodnoty a parametrov
        }T_FuncData;

typedef struct{
        char *act_varID;
        char *act_funcID;
        int rpt_size;
        char *act_rptypes;
        bool was_func;
        bool is_write;
        bool is_readln;
        bool is_ret_err;
        bool is_else;
        bool is_while;
        int begincnt; // urcuje, kedy konci funkcia alebo cely program
        int ifbegcnt; // urcuje, kedy konci prikaz if
        int whbegcnt; // urcuje, kedy konci prikaz while
        int labIDcnt; // pocitadlo, podla ktoreho sa pomenovavaju navestia
        }T_Actual;


ERROR_MSG top_down();
ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, T_vartype *expt, size_t tmems, t_varfunc_list *vflistp, t_func_list *flistp,
t_lablist *lablistp, tListOfInstr *inslistp, const char *TMPUV, Stack *s_stack, Stack *ib_stack, Stack *wb_stack);
ERROR_MSG PItems_alloc(T_ParserItem ***Ptr);
void PItems_free(T_ParserItem ***Ptr);
void free_all(T_ParserItem **p, Stack st, int stack_erase, int token_mem_free, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, t_varfunc_list *vflistp, t_lablist *lablistp, tListOfInstr *inslistp, t_func_list *flistp, int l_dispose);
int get_type(char *str,int pos);

#endif
