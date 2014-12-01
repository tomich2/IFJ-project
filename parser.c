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
  ERROR_MSG err=EVERYTHINGSOKAY; // premenna pre chybove stavy

  T_ParserItem **PItems=NULL; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  err=PItems_alloc(&PItems);
  if(err)return err;

  T_ParserItem *PItem_top; // prvok na vrchole zasobniku

  int i=0;

  T_vartype expr_type=tERR;

  T_State state; // urcuje, ktora cast programu sa analyzuje(deklaracia premennych, definicia funkcie...)
  bool is_func;

  Stack p_stack; // zasobnik
  init(&p_stack,sizeof(T_ParserItem));

  htab_t *glob_sym_table=htab_init(GTAB_SIZE); // inicializacia globalnej tabulky symbolov
  htab_t *loc_sym_table=htab_init(GTAB_SIZE); // inicializacia lokalnej tabulky symbolov

  T_Actual *Act=calloc(1,sizeof(*Act));
  Act->act_funcID=NULL;
  Act->act_varID=NULL;
  Act->act_rptypes=NULL;
  Act->was_func=false;
  Act->is_ret=false;
  Act->is_write=false;

  size_t tmem_size;

  err=get_token();
  if(err) // lexikalna chyba
  {
    free_all(PItems,p_stack,0,0,glob_sym_table,loc_sym_table,Act);
    return err;
  }
  tmem_size=sizeof(token->mem)+1;
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
        err=ExprParse(glob_sym_table,loc_sym_table,&expr_type);
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

      if(PItem_top->value.term==DtInteger) // terminal je literal, moze byt akykolvek datovy typ alebo premenna
      {
        if(token->identity==DtInteger || token->identity==DtReal || token->identity==DtString || token->identity==ID)
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
          //err=semantic(&state,glob_sym_table,loc_sym_table,Act,&expr_type,tmem_size);
          if(err!=EVERYTHINGSOKAY)
          {
            free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
            return err;
          }
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminaly sa nezhoduju=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act);
          return SYNTAX_ERR;
        }
      }
      //printf("debug..token: %s state: %d\n", token->mem,state);
    free(token->mem);
    token->mem=NULL;
    expr_type=tERR;
    err=get_token();
    if(err)
    {
      free_all(PItems,p_stack,1,0,glob_sym_table,loc_sym_table,Act);
      return err;
    }
    tmem_size=sizeof(token->mem)+1;
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

ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, T_vartype *expt, size_t tmems)
{
T_VarData *vdattmp=NULL;
T_FuncData *fdattmp=NULL;
T_VarData *cmpd=NULL;
Hitem *cmp=NULL;
ERROR_MSG err=EVERYTHINGSOKAY;
char tmp[Ac->n];

  switch(*st)
  {
    case GLOBVAR_DEK:
              vdattmp=calloc(1,sizeof(T_VarData));
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)
                      {
                        free(vdattmp);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(Ac->act_varID,token->mem);
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      err=htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      err=htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      err=htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      err=htab_new(gsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_ID:
              if(token->identity==OpLZat)break;
              if(Ac->was_func==true)htab_clear(lsymtab);
              if(token->identity==ID)
              {
                Ac->act_funcID=realloc(Ac->act_funcID,tmems);
                if(Ac->act_funcID==NULL)return INTERN_INTERPRETATION_ERR;
                strcpy(Ac->act_funcID,token->mem);
                Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*2);
                strcpy(Ac->act_rptypes,"");
                Ac->n=2;
                Ac->was_func=true;
              }
              break;

    case FUNC_PARAMS:
              vdattmp=malloc(sizeof(*vdattmp));
              if(vdattmp==NULL)return INTERN_INTERPRETATION_ERR;
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(Ac->act_varID,token->mem);
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;

                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"i");
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"s");
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"r");
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      (Ac->n)++;
                      strcat(Ac->act_rptypes,"b");
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_TYPE:
              if(token->identity==OpDek || token->identity==OpPZat || token->identity==OpKonec)break;
              fdattmp=malloc(sizeof(*fdattmp));
              if(fdattmp==NULL)return INTERN_INTERPRETATION_ERR;
              fdattmp->is_def=true;
              switch(token->identity)
              {
                case KwInteger:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(tmp,"i");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwString:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(tmp,"s");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwReal:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(tmp,"r");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwBoolean:
                      Ac->act_rptypes=realloc(Ac->act_rptypes,sizeof(char)*Ac->n);
                      if(Ac->act_rptypes==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(tmp,"b");
                      strcat(tmp,Ac->act_rptypes);
                      Ac->act_rptypes=tmp;
                      break;
                case KwForward:
                      cmp=htab_search(gsymtab,Ac->act_funcID);
                      cmpd=cmp->data;
                      cmpd->is_def=false;
                      return EVERYTHINGSOKAY;
                default: break;
              }
              fdattmp->ret_par_types=Ac->act_rptypes;
              err=htab_new(gsymtab,Ac->act_funcID,FUNCTION,fdattmp,sizeof(*fdattmp));
              if(err!=EVERYTHINGSOKAY)
              {
                free(fdattmp);
                return err;
              }
              free(Ac->act_rptypes);
              Ac->act_rptypes=NULL;
              free(fdattmp);
              break;

    case LOCVAR_DEK:
              vdattmp=malloc(sizeof(*vdattmp));
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)
                      {
                        free(vdattmp);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(Ac->act_varID,token->mem);
                      break;
                case KwInteger:
                      vdattmp->is_def=false;
                      vdattmp->type=tINTEGER;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwString:
                      vdattmp->is_def=false;
                      vdattmp->type=tSTRING;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwReal:
                      vdattmp->is_def=false;
                      vdattmp->type=tREAL;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                case KwBoolean:
                      vdattmp->is_def=false;
                      vdattmp->type=tBOOLEAN;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_BODY:
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(Ac->act_varID,token->mem);
                      if(htab_search(lsymtab,token->mem)==NULL)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,token->mem);
                        if(cmp==NULL)return SEMANTIC_ERR;
                        else if(cmp->type==FUNCTION)
                        {
                          if(Ac->is_write==false)Ac->is_ret=true;
                          else if(Ac->is_ret==false)return SEMANTIC_ERR;
                        }
                      }
                      break;
                case OpPrir:
                      cmp=(Hitem*)htab_search(lsymtab,Ac->act_varID);
                      if(cmp!=NULL)
                      {
                        cmpd=cmp->data;
                      }
                      break;
                case KwWrite:
                      Ac->is_write=true;
                      break;
                case OpPZat:
                      if(Ac->is_write==true)Ac->is_write=false;
                      break;
                case OpKonec:
                      if(*expt!=tERR)
                      {
                        cmp=htab_search(lsymtab,Ac->act_varID);
                        if(cmp==NULL)
                        {
                          cmp=htab_search(gsymtab,Ac->act_varID);
                          if(cmp==NULL)return SEMANTIC_ERR;
                          else
                          {
                            cmpd=cmp->data;
                            if((cmpd->type)!=*expt)return EXPRESSION_ERR;
                          }
                        }
                        else
                        {
                          cmpd=cmp->data;
                          if((cmpd->type)!=*expt)return EXPRESSION_ERR;
                        }
                      }
                      break;
                case KwEnd:
                      if(*expt!=tERR)
                      {
                        cmp=htab_search(lsymtab,Ac->act_varID);
                        if(cmp==NULL)
                        {
                          cmp=htab_search(gsymtab,Ac->act_varID);
                          if(cmp==NULL)return SEMANTIC_ERR;
                        }
                        else if((cmpd->type)!=*expt)return SEMANTIC_ERR;
                      }
                      break;
                default: break;
              }
              break;
    case MAIN_BODY:
              if (Ac->was_func==true)htab_clear(lsymtab);
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
      for(int j=0;j<i;j++)free((*Ptr)[j]);
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
  free(Ac->act_funcID);
  free(Ac->act_varID);
  free(Ac->act_rptypes);
  free(Ac);
  close_file();
}

int get_type(char *str,int pos)
{
  int l=strlen(str)+1;
  char tmp[l];
  strcpy(tmp,str);
  switch(tmp[pos])
  {
    case 'i':
          return tINTEGER;
    case 's':
          return tSTRING;
    case 'r':
          return tREAL;
    case 'b':
          return tBOOLEAN;
    case '\0':
          return -1;
    default:
          return -1;
  }
  return -1;
}
