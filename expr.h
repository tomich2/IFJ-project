#ifndef EXPR_H
#define EXPR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"
#include "parser.h"
#include "error.h"

typedef enum prec { R /* Right > */, L /* Left < */, E /* Equal = */, Q /* Error */ } action;

typedef enum Operator { EPlus, EMinus, EMulti, EDiv, EEqual,
			ELess, EGreater, ELorEq, EGorEq, EDiff,
			EId, ETerm,  EComma, ELpar, ERpar, EEnd} oprs;

oprs converttooprs(identita id);
int GetRule(int a, int b);
T_ParserItem *GetTerm(Stack *stack, bool hdl);
int Reduction(Stack *stack);
ERROR_MSG ExprParse();
#endif
