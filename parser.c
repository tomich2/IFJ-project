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
  ERROR_MSG err; // premenna pre chybove stavy

  T_ParserItem **PItems=NULL; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  err=PItems_alloc(&PItems);
  if(err)return err;

  T_ParserItem *PItem_top; // prvok na vrchole zasobniku

  int i=0;

  T_State state; // urcuje, ktora cast programu sa analyzuje(deklaracia premennych, definicia funkcie...)
  bool is_func;

  Stack p_stack; // zasobnik
  init(&p_stack,sizeof(T_ParserItem));

  htab_t *glob_sym_table=htab_init(GTAB_SIZE); // inicializacia globalnej tabulky symbolov
  htab_t *loc_sym_table=htab_init(GTAB_SIZE); // inicializacia lokalnej tabulky symbolov

  T_Actual *Act=malloc(sizeof(*Act));
  /*Act->act_rptypes=malloc(sizeof(char));
  strcpy(Act->act_rptypes,"\0");
  Act->n=2;
  Act->act_rptypes=realloc(Act->act_rptypes,sizeof(char)*Act->n);
  strcat(Act->act_rptypes,"r");
  Act->n++;
  Act->act_rptypes=realloc(Act->act_rptypes,sizeof(char)*Act->n);
  strcat(Act->act_rptypes,"s");
  Act->act_rptypes=realloc(Act->act_rptypes,sizeof(Act->act_rptypes)+sizeof(char));
  char tmp[Act->n];
  strcpy(tmp,"i");
  strcat(tmp,Act->act_rptypes);
  printf("rptypes: %s tmp: %s\n", Act->act_rptypes,tmp);*/

  err=get_token();
  if(err) // lexikalna chyba
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act);
    return err;
  }
  if(token->identity==EndOfFile) // syntakticka chyba=prazdny subor
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act);
    return SYNTAX_ERR;
  }
  if(get_rule(START,PItems,&state,&is_func)) // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act); // ak je prvy token nespravny=syntakticka chyba
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
      free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
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
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
          return err;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        if(get_rule(PItem_top->value.nonterm.type,PItems,&state,&is_func)) // neexistuje pravidlo=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
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
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
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
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
          return SYNTAX_ERR;
        }
      }
    free(token->mem);
    token->mem=NULL;
    err=get_token();
    if(err)
    {
      free_all(PItems,p_stack,1,0,glob_sym_table,loc_sym_table,Act);
      return err;
    }

    }
  }
  if(token->identity!=EndOfFile) // ak zdrojovy subor obsahuje nejake znaky po ukoncujucej bodke
  {
   free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act);
   return SYNTAX_ERR;

  }
  PItems_free(&PItems);
  htab_free(glob_sym_table);
  htab_free(loc_sym_table);
  free(Act);
  return EVERYTHINGSOKAY;
}

ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac)
{
T_VarData *vdattmp=NULL;
T_FuncData *fdattmp=NULL;
char tmp[Ac->n];

  switch(*st)
  {
    case GLOBVAR_DEK:
              vdattmp=malloc(sizeof(*vdattmp));
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=malloc(sizeof(token->mem));
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      Ac->act_varID=token->mem;
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      if(htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      if(htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      if(htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      if(htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_ID:
              if(token->identity==ID)
              {
                Ac->act_funcID=malloc(sizeof(token->mem));
                if(Ac->act_funcID==NULL)return INTERN_INTERPRETATION_ERR;
                Ac->act_funcID=token->mem;
                Ac->act_rptypes=malloc(sizeof(char));
                if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                strcpy(Ac->act_rptypes,"\0");
                Ac->n=2;
              }
              break;

    case FUNC_PARAMS:
              vdattmp=malloc(sizeof(*vdattmp));
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=malloc(sizeof(token->mem));
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      Ac->act_varID=token->mem;
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"i");
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"s");
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"r");
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"b");
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_TYPE:
              fdattmp=malloc(sizeof(*fdattmp));
              if(fdattmp==NULL)return INTERN_INTERPRETATION_ERR;
              fdattmp->is_def=true;
              switch(token->identity)
              {
                case KwInteger:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(Ac->act_rptypes)+sizeof(char));
                      strcpy(tmp,"i");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwString:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(Ac->act_rptypes)+sizeof(char));
                      strcpy(tmp,"s");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwReal:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(Ac->act_rptypes)+sizeof(char));
                      strcpy(tmp,"r");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwBoolean:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(Ac->act_rptypes)+sizeof(char));
                      strcpy(tmp,"b");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwForward:
                      fdattmp->is_def=false;
                      break;
                default: break;
              }
              fdattmp->ret_par_types=Ac->act_rptypes;
              if(htab_new(gsymtab,Ac->act_funcID,FUNCTION,fdattmp,sizeof(*fdattmp))!=0)return INTERN_INTERPRETATION_ERR;
              free(fdattmp);
              break;
    case LOCVAR_DEK:
              vdattmp=malloc(sizeof(*vdattmp));
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=malloc(sizeof(token->mem));
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      Ac->act_varID=token->mem;
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      if(htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp))!=0)return INTERN_INTERPRETATION_ERR;
                      free(Ac->act_varID);
                      break;
                default: break;
              }
              free(vdattmp);
              break;
    case FUNC_BODY:
              if(token->identity==KwEnd)
              {
                htab_clear(lsymtab);
                free(Ac->act_funcID);
                free(Ac->act_rptypes);
              }
              break;
    case MAIN_BODY:
              break;
    default:
            break;
  }
  return EVERYTHINGSOKAY;
}

ERROR_MSG PItems_alloc(T_ParserItem ***Ptr)
{
  *Ptr=malloc(sizeof(*Ptr)*12);
  if(*Ptr==NULL)return INTERN_INTERPRETATION_ERR;
  for(int i=0;i<=11;i++)
  {
    (*Ptr)[i]=malloc(sizeof(T_ParserItem));
    if((*Ptr)[i]==NULL)
    {
      free(*Ptr);
      return INTERN_INTERPRETATION_ERR;
    }
    (*Ptr)[i]->type=EMPTY;
  }
  return EVERYTHINGSOKAY;
}

void PItems_free(T_ParserItem ***Ptr)
{
  for(int i=0;i<=11;i++)
  {
    free((*Ptr)[i]);
  }
  free(*Ptr);
}

void free_all(T_ParserItem **p, Stack st, int stack_erase, int token_mem_free, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac)
{
  PItems_free(&p);
  if(token_mem_free==1)free(token->mem);
  if(stack_erase)S_erase(&st);
  htab_free(lsymtab);
  htab_free(gsymtab);
  free(Ac);
  close_file();
}
