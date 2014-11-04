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
  T_ParserItem **PItems; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  T_ParserItem *PItem_top; // prvok na vrchole zasobniku
  int i=0; //
  TOKEN *input; // vstupny token
  Stack p_stack; // zasobnik

  init(&p_stack,PItem_size);
  input=get_token();
  PItems=get_rule(input,START); // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems

  while(PItems[i]!=NULL) // ulozi PItems na zasobnik
  {
    push(&p_stack,PItems[i],-1);
  }

  while(!S_empty(&p_stack)) // ked bude zasobnik prazdny, syntakticka analyza konci
  {
    PItem_top=top(&p_stack);
    if(PItem_top->type==NONTERMINAL) // na vrchole zasobnika je neterminal
    {

      if(PItem_top->value.nonterm==EXPR) // ak je na vrchole zasobnika neterminal EXPR(vyraz), spracuje ho precedencna analyza
      {
        ; // precedencna (&p_stack, &input)
        free(PItem_top); // neterminal EXPR sa odstrani z pamati
        pop(&p_stack); // aj zo zasobnika
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        PItems=get_rule(input,PItem_top->value.nonterm);
        free(PItem_top); // expandovany neterminal sa odstrani z pamati
        pop(&p_stack); // aj zo zasobnika
        while(PItems[i]!=NULL && PItems!=NULL)
        {
          push(&p_stack,PItems[i],-1);
        }
      }

    }
    else // na vrchole zasobnika je terminal
    {

      if(PItem_top->value.term->identity==DtInteger) // terminal je literal, moze byt akykolvek datovy typ
      {
        if(input->identity==DtInteger || input->identity==DtChar || input->identity==DtReal || input->identity==DtBoolean || input->identity==DtString)
        {
        free(PItem_top);
        pop(&p_stack);
        }
        else // terminal nie je literal=syntakticka chyba
        {
          Error(2);
        }
      }
      else
      {
        if(input->identity==PItem_top->value.term->identity) // ak sa typ terminalu zhoduje so vstupom, odstrani sa terminal zo zasobniku a pamati a pokracuje sa na dalsi vstup
        {
          free(PItem_top);
          pop(&p_stack);
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
  free(PItems);
  return 0;
}
