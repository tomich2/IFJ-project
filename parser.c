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
  if(input==NULL)Error(99);
  get_rule(input,START,PItems); // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems

  while(PItems[i]->type!=EMPTY) // ulozi PItems na zasobnik
  {
  if(PItems[i]->type==TERMINAL)printf("PItems type: term\nPItems value: %d\n", PItems[i]->value.term);
          else if(PItems[i]->type==NONTERMINAL)printf("PItems type: nonterm\nPItems value: %d\n", PItems[i]->value.nonterm);
          else printf("PItems type: empty\n");
    push(&p_stack,PItems[i],-1);
    PItems[i]->type=EMPTY;
    i++;
  }
i=0;

  while(!S_empty(&p_stack)) // ked bude zasobnik prazdny, syntakticka analyza konci
  {
    PItem_top=top(&p_stack);
    if(PItem_top->type==NONTERMINAL) // na vrchole zasobnika je neterminal
    {

      if(PItem_top->value.nonterm==EXPR) // ak je na vrchole zasobnika neterminal EXPR(vyraz), spracuje ho precedencna analyza
      {
        if(ExprParse())Error(2);
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        get_rule(input,PItem_top->value.nonterm,PItems);
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
        while(PItems[i]->type!=EMPTY)
        {
          if(PItems[i]->type==TERMINAL)printf("PItems type: term\nPItems value: %d\n", PItems[i]->value.term);
          else if(PItems[i]->type==NONTERMINAL)printf("PItems type: nonterm\nPItems value: %d\n", PItems[i]->value.nonterm);
          else printf("PItems type: empty\n");
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
          Error(2);
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
          Error(2);
        }
      }
    free(input->mem);
    input=get_token();

    }
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
