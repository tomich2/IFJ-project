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


int top_down()
{
  T_ParserItem **PItems=NULL; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  PItems_alloc(&PItems);
  T_ParserItem *PItem_top; // prvok na vrchole zasobniku
  int i=0;
  TOKEN *input; // ukazovatel na token
  Stack p_stack; // zasobnik
  init(&p_stack,sizeof(T_ParserItem));
  input=get_token();
  if(input==NULL) // prazdny subor=syntakticka chyba
  {
    free_and_exitErr(input,PItems,p_stack,0);
  }
  if(get_rule(input,START,PItems)) // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems
  {
    free_and_exitErr(input,PItems,p_stack,0); // ak je prvy token nespravny=syntakticka chyba
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
    if(input==NULL) // zdrojovy subor je nekompletny=syntakticka chyba
    {
      free_and_exitErr(input,PItems,p_stack,1);
    }
    PItem_top=top(&p_stack);
    if(PItem_top->type==NONTERMINAL) // na vrchole zasobnika je neterminal
    {

      if(PItem_top->value.nonterm==EXPR) // ak je na vrchole zasobnika neterminal EXPR(vyraz), spracuje ho precedencna analyza
      {
        if(ExprParse()) // chyby vyraz=syntakticka chyba
        {
          free_and_exitErr(input,PItems,p_stack,1);
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        if(get_rule(input,PItem_top->value.nonterm,PItems)) // neexistuje pravidlo=syntakticka chyba
        {
          free_and_exitErr(input,PItems,p_stack,1);
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
        if(input->identity==DtInteger || input->identity==DtChar || input->identity==DtReal || input->identity==DtBoolean || input->identity==DtString)
        {
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminal nie je literal=syntakticka chyba
        {
          free_and_exitErr(input,PItems,p_stack,1);
        }
      }
      else
      {
        if(input->identity==PItem_top->value.term) // ak sa typ terminalu zhoduje so vstupom, odstrani sa terminal zo zasobniku a pamati a pokracuje sa na dalsi vstup
        {
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminaly sa nezhoduju=syntakticka chyba
        {
          free_and_exitErr(input,PItems,p_stack,1);
        }
      }
    free(input->mem);
    input=get_token();

    }
  }
  if(input!=NULL) // ak zdrojovy subor obsahuje nejake znaky po ukoncujucej bodke
  {
   free_and_exitErr(input,PItems,p_stack,0);
  }
  free(input);
  PItems_free(&PItems);
  return 0;
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

void free_and_exitErr(TOKEN *t, T_ParserItem **p, Stack st, int stack_erase)
{
  PItems_free(&p);
  free(t->mem);
  free(t);
  if(stack_erase)S_erase(&st);
  close_file();
  Error(SYNTAX_ERR);
}
