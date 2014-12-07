#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"
#include "parser.h"
#include "lexical.h"
#include "expr.h"

typedef enum prec { R /* Right > */, L /* Left < */, E /* Equal = */, Q /* Error */ } action;

typedef enum Operator { EPlus, EMinus, EMulti, EDiv, EEqual,
			ELess, EGreater, ELorEq, EGorEq, EDiff,
			EId, ETerm, EComma=12, ELpar=13, ERpar=14, EEnd=15, ETermInt=16,ETermString=17, ETermReal=18, ETermBool=19 } oprs;

typedef struct sym_tabs
{
	htab_t *glob;
	htab_t *loc;
	tListOfInstr *InstL;
	bool is_cmp;
	bool is_comm;
	bool is_func;
} Tabs;


oprs converttooprs(identita id, bool tab);
int GetRule(int a, int b);
T_ParserItem *GetTerm(Stack *stack, bool hdl);
int Reduction(Stack *stack, T_ParserItem *in, Tabs *STab);
ERROR_MSG ExprParse( htab_t *glob, htab_t *loc, T_vartype *dt, tListOfInstr *InstL);
ERROR_MSG ExprSem(int rule, nont *op1, nont *op2, Tabs *STab);
ERROR_MSG MakeVariable(Variable **a, T_vartype type, void *data);
#endif
