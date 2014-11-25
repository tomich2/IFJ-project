// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include "lexical.h"
#include "parser.h"
#include "rules.h"
#include "stack.h"
#include "error.h"
#include "expr.h"


ERROR_MSG top_down()
{
  ERROR_MSG err;
  T_ParserItem **PItems=NULL; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  PItems_alloc(&PItems);
  T_ParserItem *PItem_top; // prvok na vrchole zasobniku
  int i=0;
  T_State state;
  Stack p_stack; // zasobnik
  init(&p_stack,sizeof(T_ParserItem));
  htab_t *glob_sym_table=htab_init(GTAB_SIZE);
  err=get_token();
  if(err) // lexikalna chyba alebo prazdny subor=syntakticka chyba
  {
    free_all(PItems,p_stack,0,glob_sym_table);
    return (token->identity==EndOfFile) ? SYNTAX_ERR : err;
  }
  if(get_rule(START,PItems)) // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems
  {
    free_all(PItems,p_stack,0,glob_sym_table); // ak je prvy token nespravny=syntakticka chyba
    return SYNTAX_ERR;
  }

  while(PItems[i]->type!=EMPTY) // ulozi PItems na zasobnik
  {
    push(&p_stack,PItems[i],-1);
    PItems[i]->type=EMPTY;
    i++;
  }
i=0;

  while(!S_empty(&p_stack)) // ked bude zasobnik prazdny, syntakticka analyza konci
  {
    if(token->identity==EndOfFile) // zdrojovy subor je nekompletny=syntakticka chyba
    {
      free_all(PItems,p_stack,1,glob_sym_table);
      return SYNTAX_ERR;
    }
    PItem_top=top(&p_stack);
    if(PItem_top->type==NONTERMINAL) // na vrchole zasobnika je neterminal
    {

      if(PItem_top->value.nonterm.type==EXPR) // ak je na vrchole zasobnika neterminal EXPR(vyraz), spracuje ho precedencna analyza
      {
        err=ExprParse();
        if(err) // chybny vyraz=syntakticka chyba
        {
          free_all(PItems,p_stack,1,glob_sym_table);
          return err;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        if(get_rule(PItem_top->value.nonterm.type,PItems)) // neexistuje pravidlo=syntakticka chyba
        {
          free_all(PItems,p_stack,1,glob_sym_table);
          return SYNTAX_ERR;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
        while(PItems[i]->type!=EMPTY)
        {
          push(&p_stack,PItems[i],-1);
          PItems[i]->type=EMPTY;
          i++;
        }
        i=0;
      }

    }
    else // na vrchole zasobnika je terminal
    {

      if(PItem_top->value.term==DtInteger) // terminal je literal, moze byt akykolvek datovy typ
      {
        if(token->identity==DtInteger || token->identity==DtReal || token->identity==DtString)
        {
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminal nie je literal=syntakticka chyba
        {
          free_all(PItems,p_stack,1,glob_sym_table);
          return SYNTAX_ERR;
        }
      }
      else
      {
        if(token->identity==PItem_top->value.term) // ak sa typ terminalu zhoduje so vstupom, odstrani sa terminal zo zasobniku a pokracuje sa na dalsi vstup
        {
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminaly sa nezhoduju=syntakticka chyba
        {
          free_all(PItems,p_stack,1,glob_sym_table);
          return SYNTAX_ERR;
        }
      }
    free(token->mem);
    token->mem=NULL;
    err=get_token();
    if(err)
    {
      free_all(PItems,p_stack,1,glob_sym_table);
      return err;
    }

    }
  }
  if(token->identity!=EndOfFile) // ak zdrojovy subor obsahuje nejake znaky po ukoncujucej bodke
  {
   free_all(PItems,p_stack,0,glob_sym_table);
   return SYNTAX_ERR;

  }
  PItems_free(&PItems);
  htab_free(glob_sym_table);
  return EVERYTHINGSOKAY;
}

void PItems_alloc(T_ParserItem ***Ptr)
{
  *Ptr=malloc(sizeof(*Ptr)*11);
  for(int i=0;i<=10;i++)
  {
    (*Ptr)[i]=malloc(sizeof(T_ParserItem));
    if((*Ptr)[i]==NULL)Error(99);
    (*Ptr)[i]->type=EMPTY;
  }
}

void PItems_free(T_ParserItem ***Ptr)
{
  for(int i=0;i<=10;i++)
  {
    free((*Ptr)[i]);
  }
  free(*Ptr);
}

void free_all(T_ParserItem **p, Stack st, int stack_erase, htab_t *gsymtab)
{
  PItems_free(&p);
  if(token->mem!=NULL)free(token->mem);
  if(stack_erase)S_erase(&st);
  htab_free(gsymtab);
  close_file();
}
