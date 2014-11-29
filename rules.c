// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "rules.h"
#include "error.h"
#include "stack.h"

                                  // 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20
int LLTable[LL_TERMS][LL_NONTERMS]={{1, 2, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},     // var
                                    {0, 0, 4, 6, 0, 0, 11, 0, 0, 0, 21, 24, 0, 29, 0, 0, 0, 0, 34, 0}, // id
                                    {1, 3, 0, 5, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // function
                                    {0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0},     // integer
                                    {0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0},     // real
                                    {0, 0, 0, 0, 0, 0, 0, 0, 17, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0},     // string
                                    {0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 0, 0 ,0, 0, 0, 0, 0, 0, 0},     // boolean
                                    {1, 3, 0, 5, 8, 10, 0, 0, 0, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},    // begin
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 22, 0, 0, 0, 0, 0, 0, 0},    // end
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 37},     // comma
                                    {0, 0, 0, 0, 0, 0, 0, 13, 0, 0, 0, 0, 23, 0, 0, 0, 0, 0, 0, 0},    // semicolon
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 26, 0, 0, 0, 31, 0, 0, 0, 0},   // if
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 25, 0, 0, 30, 0, 0, 0, 0, 0},   // while
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0},     // literal
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 27, 0, 0, 0, 0, 32, 0, 0, 0, 0},// readln
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 28, 0, 0, 0, 0, 0, 33, 0, 0},   // write
                                    {0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // forward
                                    {0, 0, 0, 0, 0, 0, 12, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36}};  // right bracket
T_terms token_to_term()
{
    switch(token->identity)
    {
      case ID:
        return id_;
      case KwBegin:
        return begin_;
      case KwBoolean:
        return boolean_;
      case KwEnd:
        return end_;
      case KwForward:
        return forward_;
      case KwFunction:
        return function_;
      case KwIf:
        return if_;
      case KwInteger:
        return integer_;
      case KwReadln:
        return readln_;
      case KwReal:
        return real_;
      case KwString:
        return string_;
      case KwVar:
        return var_;
      case KwWhile:
        return while_;
      case KwWrite:
        return write_;
      case OpKonec:
        return semicolon_;
      case OpCiarka:
        return comma_;
      case OpPZat:
        return r_bracket_;
      case DtInteger:
       case DtReal:
        case DtString:
         return literal_;
      default:
        break;
    }
  return error;
}


int get_rule(T_nonterms nonterm, T_ParserItem **PItem_Arr, T_State *st, bool *is_f)
{
  int rule;
  T_terms term;
  term=token_to_term();
  rule=LLTable[term][nonterm];
  switch(rule)
  {
    case -1:
    case 0:
          return 1; // pravidlo neexistuje
    case 1: // 1. <START> → <DEF_VAR> <FUNC> <BODY>.
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=OpBodka;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=BODY;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm.type=FUNC;
          PItem_Arr[3]->type=NONTERMINAL;
          PItem_Arr[3]->value.nonterm.type=DEF_VAR;
          *is_f=false;
          break;
    case 2: // 2. <DEF_VAR> → var <VAR>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=VAR;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=KwVar;
          if(*is_f==false)*st=GLOBVAR_DEK;
          else *st=LOCVAR_DEK;
          break;
    case 4: // 4. <VAR> → id :<TYPE>; <VAR_N>
    case 6: // 6. <VAR_N> →  id :<TYPE>; <VAR_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=VAR_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=OpKonec;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm.type=TYPE;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=OpDek;
          PItem_Arr[4]->type=TERMINAL;
          PItem_Arr[4]->value.term=ID;
          break;
    case 7: // 7. <FUNC> →  function id(<PARAM>) :<TYPE>;  <FORWARD>; <FUNC>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=FUNC;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=OpKonec;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm.type=FORWARD;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=OpKonec;
          PItem_Arr[4]->type=NONTERMINAL;
          PItem_Arr[4]->value.nonterm.type=TYPE;
          PItem_Arr[5]->type=TERMINAL;
          PItem_Arr[5]->value.term=OpDek;
          PItem_Arr[6]->type=TERMINAL;
          PItem_Arr[6]->value.term=OpPZat;
          PItem_Arr[7]->type=NONTERMINAL;
          PItem_Arr[7]->value.nonterm.type=PARAM;
          PItem_Arr[8]->type=TERMINAL;
          PItem_Arr[8]->value.term=OpLZat;
          PItem_Arr[9]->type=TERMINAL;
          PItem_Arr[9]->value.term=ID;
          PItem_Arr[10]->type=TERMINAL;
          PItem_Arr[10]->value.term=KwFunction;
          *is_f=true;
          *st=FUNC_ID;
          break;
    case 9: // 9. <FORWARD> → forward
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwForward;
          break;
    case 10: // 10. <FORWARD> →  <DEF_VAR> <BODY>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=BODY;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=DEF_VAR;
          break;
    case 11: // 11. <PARAM> → id :<TYPE> <PARAM_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=PARAM_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=TYPE;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=OpDek;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=ID;
          *st=FUNC_PARAMS;
          break;
    case 13: // 13. <PARAM_N> → ;  id :<TYPE> <PARAM_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=PARAM_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=TYPE;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=OpDek;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=ID;
          PItem_Arr[4]->type=TERMINAL;
          PItem_Arr[4]->value.term=OpKonec;
          break;
    case 15: // 15. <TYPE> → integer
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwInteger;
          break;
    case 16: // 16. <TYPE> → real
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwReal;
          break;
    case 17: // 17. <TYPE> → string
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwString;
          break;
    case 18: // 18. <TYPE> → boolean
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwBoolean;
          break;
    case 19: // 19. <BODY> → begin <STAT_S> end
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=KwEnd;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=STAT_S;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=KwBegin;
          if(*is_f==false)*st=MAIN_BODY;
          else *st=FUNC_BODY;
          *is_f=false;
          break;
    case 21: // 21. <STAT_S> → <STAT> <STAT_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=STAT_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=STAT;
          break;
    case 23: // 23. <STAT_N> → ; <STAT> <STAT_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=STAT_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=STAT;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=OpKonec;
          break;
    case 24: // 24. <STAT> → <ASSIGN>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=ASSIGN;
          break;
    case 25: // 25. <STAT> → <WHILE>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=WHILE;
          break;
    case 26: // 26. <STAT> → <IFELSE>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=IFELSE;
          break;
    case 27: // 27. <STAT> → <READ>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=READ;
          break;
    case 28: // 28. <STAT> → <WRITE>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=WRITE;
          break;
    case 29: // 29. <ASSIGN> → id := <EXPR>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=EXPR;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=OpPrir;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=ID;
          break;
    case 30: // 30. <WHILE> → while <EXPR> do <BODY>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=BODY;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=KwDo;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm.type=EXPR;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=KwWhile;
          break;
    case 31: // 31. <IFELSE> → if <EXPR> then <BODY> else <BODY>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=BODY;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=KwElse;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm.type=BODY;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=KwThen;
          PItem_Arr[4]->type=NONTERMINAL;
          PItem_Arr[4]->value.nonterm.type=EXPR;
          PItem_Arr[5]->type=TERMINAL;
          PItem_Arr[5]->value.term=KwIf;
          break;
    case 32: // 32. <READ> → readln(id)
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=OpPZat;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=ID;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=OpLZat;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=KwReadln;
          break;
    case 33: // 33. <WRITE> → write(<TERM>)
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term=OpPZat;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm.type=TERM;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term=OpLZat;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term=KwWrite;
          break;
    case 34: // 34. <TERM> → id <TERM_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=TERM_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=ID;
          break;
    case 35: // 35. <TERM> → literal <TERM_N>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=TERM_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=DtInteger; // DtInteger v tomto pripade znamena literal, t.j. akykolvek datovy typ
          break;
    case 37: // 37. <TERM_N> → , <TERM>
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm.type=TERM;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term=OpCiarka;
          break;
    // epsilon pravidla
    case 12:
        case 14:
              *st=FUNC_TYPE;
    case 3:
     case 5:
      case 8:

         case 20:
          case 22:
           case 36:
                  break;
    default: break;
  }
return 0;
}
