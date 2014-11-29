// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef RULES_H_
#define RULES_H_

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define LL_TERMS 19
#define LL_NONTERMS 21

typedef enum {var_, id_, function_, integer_, real_, string_, boolean_, begin_, end_, comma_,
                    semicolon_, if_, while_, literal_, readln_, write_, forward_, r_bracket_, error=-1}T_terms;

typedef enum {START, DEF_VAR, VAR, VAR_N, FUNC, FORWARD, PARAM, PARAM_N, TYPE, BODY, STAT_S, STAT, STAT_N,
                       ASSIGN, WHILE, IFELSE, READ, WRITE, TERM, TERM_N, EXPR, ERROR=-1}T_nonterms;

T_terms token_to_term();
int get_rule(T_nonterms nonterm, T_ParserItem **PItem_Arr, T_State *st, bool *is_f);


#endif

