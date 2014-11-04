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
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 27, 0, 0, 32, 0, 0, 0},         // readln
                                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 28, 0, 0, 0, 33, 0, 0},         // write
                                    {0, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},      // forward
                                    {0, 0, 0, 0, 0, 0, 12, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 36}};  // right bracket

T_terms token_to_term(TOKEN *t)
{
    switch(t->identity)
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
      case OpPZat:
        return r_bracket_;
      case DtInteger:
        case DtChar:
          case DtReal:
            case DtBoolean:
             case DtString:
              return literal_;
      default:
        break;
    }
  return error;
}


T_ParserItem ** get_rule(TOKEN *token,T_nonterms nonterm)
{
  int rule;
  T_terms term;
  term=token_to_term(token);
  rule=LLTable[term][nonterm];
  T_ParserItem **PItem_Arr;
  switch(rule)
  {
    case 1: // 1. <START> → <DEF_VAR> <FUNC> <BODY>.
          PItem_Arr=malloc(PItem_size*4);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=OpBodka;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=BODY;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm=FUNC;
          PItem_Arr[3]->type=NONTERMINAL;
          PItem_Arr[3]->value.nonterm=DEF_VAR;
          return PItem_Arr;
    case 2: // 2. <DEF_VAR> → var <VAR>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=DEF_VAR;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=KwVar;
          return PItem_Arr;
    case 4: // 4. <VAR> → id :<TYPE>; <VAR_N>
    case 6: // 6. <VAR_N> →  id :<TYPE>; <VAR_N>
          PItem_Arr=malloc(PItem_size*5);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=VAR_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=OpKonec;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm=TYPE;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=OpDek;
          PItem_Arr[4]->type=TERMINAL;
          PItem_Arr[4]->value.term->identity=ID;
          return PItem_Arr;
    case 7: // 7. <FUNC> →  function id(<PARAM>) :<TYPE>;  <FORWARD>; <FUNC>
          PItem_Arr=malloc(PItem_size*11);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=FUNC;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=OpKonec;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm=FORWARD;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=OpKonec;
          PItem_Arr[4]->type=NONTERMINAL;
          PItem_Arr[4]->value.nonterm=TYPE;
          PItem_Arr[5]->type=TERMINAL;
          PItem_Arr[5]->value.term->identity=OpDek;
          PItem_Arr[6]->type=TERMINAL;
          PItem_Arr[6]->value.term->identity=OpPZat;
          PItem_Arr[7]->type=NONTERMINAL;
          PItem_Arr[7]->value.nonterm=PARAM;
          PItem_Arr[8]->type=TERMINAL;
          PItem_Arr[8]->value.term->identity=OpLZat;
          PItem_Arr[9]->type=TERMINAL;
          PItem_Arr[9]->value.term->identity=ID;
          PItem_Arr[10]->type=TERMINAL;
          PItem_Arr[10]->value.term->identity=KwFunction;
          return PItem_Arr;
    case 9: // 9. <FORWARD> → forward
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwForward;
          return PItem_Arr;
    case 10: // 10. <FORWARD> →  <DEF_VAR> <BODY>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=BODY;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=DEF_VAR;
          return PItem_Arr;
    case 11: // 11. <PARAM> → id :<TYPE> <PARAM_N>
          PItem_Arr=malloc(PItem_size*4);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=PARAM_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=TYPE;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=OpDek;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=ID;
          return PItem_Arr;
    case 13: // 13. <PARAM_N> → ;  id :<TYPE> <PARAM_N>
          PItem_Arr=malloc(PItem_size*5);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=PARAM_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=TYPE;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=OpDek;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=ID;
          PItem_Arr[4]->type=TERMINAL;
          PItem_Arr[4]->value.term->identity=OpKonec;
          return PItem_Arr;
    case 15: // 15. <TYPE> → integer
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwInteger;
          return PItem_Arr;
    case 16: // 16. <TYPE> → real
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwReal;
          return PItem_Arr;
    case 17: // 17. <TYPE> → string
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwString;
          return PItem_Arr;
    case 18: // 18. <TYPE> → boolean
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwBoolean;
          return PItem_Arr;
    case 19: // 19. <BODY> → begin <STAT_S> end
          PItem_Arr=malloc(PItem_size*3);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=KwEnd;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=STAT_S;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=KwBegin;
          return PItem_Arr;
    case 21: // 21. <STAT_S> → <STAT> <STAT_N>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=STAT_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=STAT;
          return PItem_Arr;
    case 23: // 23. <STAT_N> → ; <STAT> <STAT_N>
          PItem_Arr=malloc(PItem_size*3);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=STAT_N;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm=STAT;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=OpKonec;
          return PItem_Arr;
    case 24: // 24. <STAT> → <ASSIGN>
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=ASSIGN;
          return PItem_Arr;
    case 25: // 25. <STAT> → <WHILE>
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=WHILE;
          return PItem_Arr;
    case 26: // 26. <STAT> → <IFELSE>
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=IFELSE;
          return PItem_Arr;
    case 27: // 27. <STAT> → <READ>
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=READ;
          return PItem_Arr;
    case 28: // 28. <STAT> → <WRITE>
          PItem_Arr=malloc(PItem_size);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=WRITE;
          return PItem_Arr;
    case 29: // 29. <ASSIGN> → id := <EXPR>
          PItem_Arr=malloc(PItem_size*3);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=EXPR;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=OpPrir;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=ID;
          return PItem_Arr;
    case 30: // 30. <WHILE> → while <EXPR> do <BODY>
          PItem_Arr=malloc(PItem_size*4);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=BODY;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=KwDo;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm=EXPR;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=KwWhile;
          return PItem_Arr;
    case 31: // 31. <IFELSE> → if <EXPR> then <BODY> else <BODY>
          PItem_Arr=malloc(PItem_size*6);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=BODY;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=KwElse;
          PItem_Arr[2]->type=NONTERMINAL;
          PItem_Arr[2]->value.nonterm=BODY;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=KwThen;
          PItem_Arr[4]->type=NONTERMINAL;
          PItem_Arr[4]->value.nonterm=EXPR;
          PItem_Arr[5]->type=TERMINAL;
          PItem_Arr[5]->value.term->identity=KwIf;
          return PItem_Arr;
    case 32: // 32. <READ> → readln(id)
          PItem_Arr=malloc(PItem_size*4);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=OpPZat;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=ID;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=OpLZat;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=KwReadln;
          return PItem_Arr;
    case 33: // 33. <WRITE> → write(<TERM>)
          PItem_Arr=malloc(PItem_size*4);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=TERMINAL;
          PItem_Arr[0]->value.term->identity=OpPZat;
          PItem_Arr[1]->type=NONTERMINAL;
          PItem_Arr[1]->value.nonterm-=TERM;
          PItem_Arr[2]->type=TERMINAL;
          PItem_Arr[2]->value.term->identity=OpLZat;
          PItem_Arr[3]->type=TERMINAL;
          PItem_Arr[3]->value.term->identity=KwWrite;
          return PItem_Arr;
    case 34: // 34. <TERM> → id <TERM_N>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=TERM_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=ID;
          return PItem_Arr;
    case 35: // 35. <TERM> → literal <TERM_N>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=TERM_N;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=DtInteger; // DtInteger v tomto pripade znamena literal, t.j. akykolvek datovy typ
          return PItem_Arr;
    case 37: // 37. <TERM_N> → , <TERM>
          PItem_Arr=malloc(PItem_size*2);
          if(PItem_Arr==NULL)Error(99);
          PItem_Arr[0]->type=NONTERMINAL;
          PItem_Arr[0]->value.nonterm=TERM;
          PItem_Arr[1]->type=TERMINAL;
          PItem_Arr[1]->value.term->identity=OpCiarka;
          return PItem_Arr;
    default: return NULL;
  }
}
