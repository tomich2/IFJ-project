// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
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
  Act->act_rptypes=calloc(1,sizeof(char)*MAX_RPTYPES);
  Act->was_func=false;
  Act->is_write=false;
  Act->is_readln=false;
  Act->is_ret_err=true;
  Act->rpt_size=2;
  Act->begincnt=0;
  Act->labIDcnt=1;

  size_t tmem_size;

  t_varfunc_list *vflist=malloc(sizeof(*vflist));
  varfuncL_init(vflist);
  t_func_list *flist=malloc(sizeof(*flist));
  funcL_init(flist);
  t_lablist *lablist=malloc(sizeof(*lablist));
  labL_init(lablist);
  tListOfInstr *inslist=malloc(sizeof(*inslist));
  listInit(inslist);

  err=get_token();
  if(err!=EVERYTHINGSOKAY) // lexikalna chyba
  {
    free_all(PItems,p_stack,0,0,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
    return err;
  }
  if(token->identity==EndOfFile) // syntakticka chyba=prazdny subor
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
    return SYNTAX_ERR;
  }
    tmem_size=strlen(token->mem)+1;
  if(get_rule(START,PItems,&state,&is_func)) // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1); // ak je prvy token nespravny=syntakticka chyba
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
      free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
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
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
          return err;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        if(get_rule(PItem_top->value.nonterm.type,PItems,&state,&is_func)) // neexistuje pravidlo=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
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

      if(PItem_top->value.term.type==DtInteger) // terminal je literal, moze byt akykolvek datovy typ alebo premenna
      {
        if(token->identity==DtInteger || token->identity==DtReal || token->identity==DtString || token->identity==ID)
        {
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminal nie je literal=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
          return SYNTAX_ERR;
        }
      }
      else
      {
        if(token->identity==PItem_top->value.term.type) // ak sa typ terminalu zhoduje so vstupom, odstrani sa terminal zo zasobniku a pokracuje sa na dalsi vstup
        {
          err=semantic(&state,glob_sym_table,loc_sym_table,Act,&expr_type,tmem_size,vflist,flist,lablist,inslist);
          if(err!=EVERYTHINGSOKAY)
          {
            free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
            return err;
          }
          if(Act->begincnt==0 && state==FUNC_BODY)is_func=false;
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminaly sa nezhoduju=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
          return SYNTAX_ERR;
        }
      }
      //printf("debug..token: %s state: %d\n", token->mem,state);
    free(token->mem);
    token->mem=NULL;
    expr_type=tERR;
    err=get_token();
    if(err!=EVERYTHINGSOKAY)
    {
      free_all(PItems,p_stack,1,0,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
      return err;
    }
    if(token->mem!=NULL)tmem_size=strlen(token->mem)+1;
    }
  }
  if(token->identity!=EndOfFile) // ak zdrojovy subor obsahuje nejake znaky po ukoncujucej bodke
  {
   free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
   return SYNTAX_ERR;
  }
  free_all(PItems,p_stack,0,0,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist,1);
  return EVERYTHINGSOKAY;
}

ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, T_vartype *expt, size_t tmems, t_varfunc_list *vflistp, t_func_list *flistp, t_lablist *lablistp, tListOfInstr *inslistp)
{
T_VarData *vdattmp=NULL;
T_FuncData *fdattmp=NULL;
T_VarData *vcmpd=NULL;
T_FuncData *fcmpd=NULL;
tListItem *ins_adress=NULL;
Hitem *cmp=NULL;
ERROR_MSG err=EVERYTHINGSOKAY;
char tmp[Ac->rpt_size];
Variable *varA=NULL;
Variable *varB=NULL;
if(Ac->rpt_size==MAX_RPTYPES)
{
  size_t s=sizeof(Ac->act_rptypes);
  Ac->act_rptypes=realloc(Ac->act_rptypes,s+10);
}
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
                      if(varfuncL_insertlast(vflistp,NULL,IDENTIFIER,Ac->act_varID,tINTEGER)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(varfuncL_insertlast(vflistp,NULL,IDENTIFIER,Ac->act_varID,tSTRING)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(varfuncL_insertlast(vflistp,NULL,IDENTIFIER,Ac->act_varID,tREAL)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(varfuncL_insertlast(vflistp,NULL,IDENTIFIER,Ac->act_varID,tBOOLEAN)!=0)return INTERN_INTERPRETATION_ERR;
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_ID:
              if(token->identity==OpLZat || token->identity==KwFunction)break;
              if(Ac->was_func==true)
              {
                Ac->is_ret_err=true;
                htab_clear(lsymtab);
              }
              if(token->identity==ID)
              {
                Ac->act_funcID=realloc(Ac->act_funcID,tmems);
                if(Ac->act_funcID==NULL)return INTERN_INTERPRETATION_ERR;
                strcpy(Ac->act_funcID,token->mem);
                strcpy(Ac->act_rptypes,"");
                Ac->rpt_size=2;
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
                      if(strcmp(Ac->act_funcID,Ac->act_varID)==0)
                      {
                        free(vdattmp);
                        return SEMANTIC_ERR;
                      }
                      break;
                case KwInteger:
                      vdattmp->is_def=true;
                      vdattmp->type=tINTEGER;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      if(funcL_insertfirst(flistp,Ac->act_varID,tINTEGER,true)!=0)return INTERN_INTERPRETATION_ERR;
                      Ac->rpt_size++;
                      strcat(Ac->act_rptypes,"i");
                      break;
                case KwString:
                      vdattmp->is_def=true;
                      vdattmp->type=tSTRING;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      if(funcL_insertfirst(flistp,Ac->act_varID,tSTRING,true)!=0)return INTERN_INTERPRETATION_ERR;
                      Ac->rpt_size++;
                      strcat(Ac->act_rptypes,"s");
                      break;
                case KwReal:
                      vdattmp->is_def=true;
                      vdattmp->type=tREAL;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      if(funcL_insertfirst(flistp,Ac->act_varID,tREAL,true)!=0)return INTERN_INTERPRETATION_ERR;
                      Ac->rpt_size++;
                      strcat(Ac->act_rptypes,"r");
                      break;
                case KwBoolean:
                      vdattmp->is_def=true;
                      vdattmp->type=tBOOLEAN;
                      err=htab_new(lsymtab,Ac->act_varID,IDENTIFIER,vdattmp,sizeof(*vdattmp));
                      if(err!=EVERYTHINGSOKAY)
                      {
                        free(vdattmp);
                        return err;
                      }
                      if(funcL_insertfirst(flistp,Ac->act_varID,tBOOLEAN,true)!=0)return INTERN_INTERPRETATION_ERR;
                      Ac->rpt_size++;
                      strcat(Ac->act_rptypes,"b");
                      break;
                default: break;
              }
              free(vdattmp);
              break;

    case FUNC_TYPE:
              if(token->identity==OpDek || token->identity==OpPZat || token->identity==OpKonec)return EVERYTHINGSOKAY;
              fdattmp=malloc(sizeof(*fdattmp));
              if(fdattmp==NULL)return INTERN_INTERPRETATION_ERR;
              fdattmp->ret_par_types=malloc(strlen(Ac->act_rptypes)+2);
              if(fdattmp->ret_par_types==NULL)
              {
                free(fdattmp);
                return INTERN_INTERPRETATION_ERR;
              }
              T_vartype ftype;
              switch(token->identity)
              {
                case KwInteger:
                      strcpy(tmp,"i");
                      strcat(tmp,Ac->act_rptypes);
                      strcpy(Ac->act_rptypes,tmp);
                      ftype=tINTEGER;
                      break;
                case KwString:
                      strcpy(tmp,"s");
                      strcat(tmp,Ac->act_rptypes);
                      strcpy(Ac->act_rptypes,tmp);
                      ftype=tSTRING;
                      break;
                case KwReal:
                      strcpy(tmp,"r");
                      strcat(tmp,Ac->act_rptypes);
                      strcpy(Ac->act_rptypes,tmp);
                      ftype=tREAL;
                      break;
                case KwBoolean:
                      strcpy(tmp,"b");
                      strcat(tmp,Ac->act_rptypes);
                      strcpy(Ac->act_rptypes,tmp);
                      ftype=tBOOLEAN;
                      break;
                case KwForward:
                      cmp=htab_search(gsymtab,Ac->act_funcID);
                      fcmpd=cmp->data;
                      fcmpd->is_def=false;
                      free(fdattmp->ret_par_types);
                      free(fdattmp);
                      return EVERYTHINGSOKAY;
                default: break;
              }
              strcpy(fdattmp->ret_par_types,Ac->act_rptypes);
              fdattmp->is_ret=false;
              cmp=htab_search(gsymtab,Ac->act_funcID);
              if(cmp!=NULL)
              {
                fcmpd=cmp->data;
                if(fcmpd->is_def==false)
                {
                  if(strcmp(fcmpd->ret_par_types,Ac->act_rptypes)!=0)
                  {
                    free(fdattmp->ret_par_types);
                    free(fdattmp);
                    return SEMANTIC_ERR;
                  }
                  else
                  {
                    fcmpd->is_def=true;
                    free(fdattmp->ret_par_types);
                    free(fdattmp);
                    return EVERYTHINGSOKAY;
                  }
                }
                else
                {
                  free(fdattmp->ret_par_types);
                  free(fdattmp);
                  return SEMANTIC_ERR;
                }
              }
              else
              {
                fdattmp->is_def=true;
                err=htab_new(gsymtab,Ac->act_funcID,FUNCTION,fdattmp,sizeof(*fdattmp));
                if(err!=EVERYTHINGSOKAY)
                {
                  free(fdattmp->ret_par_types);
                  free(fdattmp);
                  return err;
                }
                if(varfuncL_insertlast(vflistp,flistp,FUNCTION,Ac->act_funcID,ftype)!=0)return INTERN_INTERPRETATION_ERR;
                free(fdattmp);
              }
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
                      if(strcmp(Ac->act_funcID,Ac->act_varID)==0)
                      {
                        free(vdattmp);
                        return SEMANTIC_ERR;
                      }
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
                      if(funcL_insertfirst(flistp,Ac->act_varID,tINTEGER,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(flistp,Ac->act_varID,tSTRING,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(flistp,Ac->act_varID,tREAL,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(flistp,Ac->act_varID,tBOOLEAN,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      cmp=(Hitem*)htab_search(lsymtab,token->mem);
                      if(cmp==NULL)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,token->mem);
                        if(cmp==NULL)return SEMANTIC_ERR;
                        else
                        {
                          if(cmp->type==FUNCTION)
                          {
                            if(strcmp(cmp->key,Ac->act_funcID)!=0)return SEMANTIC_ERR;
                            fcmpd=cmp->data;
                            if(Ac->is_write==true && fcmpd->is_ret==false)return SEMANTIC_ERR;
                            else if(Ac->is_readln==true)
                            {
                              if(get_type(fcmpd->ret_par_types,0)==tBOOLEAN)return EXPRESSION_ERR;
                              fcmpd->is_ret=true;
                              Ac->is_ret_err=false;
                            }
                          }
                          else
                          {
                            vcmpd=cmp->data;
                            if(Ac->is_write==true && vcmpd->is_def==false)return SEMANTIC_ERR;
                            if(Ac->is_readln==true)
                            {
                              if(vcmpd->type==tBOOLEAN)return EXPRESSION_ERR;
                              vcmpd->is_def=true;
                              Ac->is_ret_err=false;
                            }
                          }
                        }
                      }
                      else
                      {
                        vcmpd=cmp->data;
                        if(Ac->is_write==true)
                        {
                          if(vcmpd->is_def==false)return SEMANTIC_ERR;
                          varA=malloc(sizeof(*varA));
                          if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                          varA->data.s=malloc(strlen(Ac->act_varID)+1);
                          if(varA->data.s==NULL)
                          {
                            free(varA);
                            return INTERN_INTERPRETATION_ERR;
                          }
                          varA->type=vcmpd->type;
                          generator(inslistp,I_PRINT,varA,NULL,NULL);
                        }
                        if(Ac->is_readln==true)
                        {
                          if(vcmpd->type==tBOOLEAN)return EXPRESSION_ERR;
                          vcmpd->is_def=true;
                          generator(inslistp,I_READ,NULL,NULL,Ac->act_varID); // instrukcia pre readln ******
                        }
                      }
                      break;
                case OpPrir:
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->data.s=malloc(TMPLEN*sizeof(char));
                      if(varA->data.s==NULL)
                      {
                        free(varA);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      varA->data.s=TMPU;
                      cmp=(Hitem*)htab_search(lsymtab,Ac->act_varID);
                      if(cmp==NULL)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                        if(cmp->type==IDENTIFIER)
                        {
                          vcmpd=cmp->data;
                          vcmpd->is_def=true;
                          varA->type=vcmpd->type;
                        }
                        else if(cmp->type==FUNCTION)
                        {
                          fcmpd=cmp->data;
                          fcmpd->is_ret=true;
                          Ac->is_ret_err=false;
                          varA->type=get_type(fcmpd->ret_par_types,0);
                        }
                      }
                      else
                      {
                        vcmpd=cmp->data;
                        vcmpd->is_def=true;
                        varA->type=vcmpd->type;
                      }
                      generator(inslistp,I_ASSIGN,varA,NULL,Ac->act_varID);
                      break;
                case KwWrite:
                      Ac->is_write=true;
                      break;
                case KwReadln:
                      Ac->is_readln=true;
                      break;
                case OpPZat:
                      if(Ac->is_write==true)Ac->is_write=false;
                      if(Ac->is_readln==true)Ac->is_readln=false;
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
                            if(cmp->type==IDENTIFIER)
                            {
                              vcmpd=cmp->data;
                              if((vcmpd->type)!=*expt)return EXPRESSION_ERR;
                            }
                            else if(cmp->type==FUNCTION)
                            {
                              fcmpd=cmp->data;
                              if(get_type(fcmpd->ret_par_types,0)!=*expt)return EXPRESSION_ERR;
                            }
                          }
                        }
                        else
                        {
                          vcmpd=cmp->data;
                          if((vcmpd->type)!=*expt)return EXPRESSION_ERR;
                        }
                      }
                      break;
                case KwBegin:
                      if(Ac->begincnt==0)
                      {
                        ins_adress=generator(inslistp,I_LABEL,NULL,NULL,NULL);
                        labL_insertlast(lablistp,ins_adress,Ac->labIDcnt);
                      }
                      Ac->begincnt++;
                      break;
                case KwEnd:
                      Ac->begincnt--;
                      if(*expt!=tERR)
                      {
                        cmp=htab_search(lsymtab,Ac->act_varID);
                        if(cmp==NULL)
                        {
                          cmp=htab_search(gsymtab,Ac->act_varID);
                          if(cmp==NULL)return SEMANTIC_ERR;
                          else
                          {
                            if(cmp->type==IDENTIFIER)
                            {
                              vcmpd=cmp->data;
                              if((vcmpd->type)!=*expt)return EXPRESSION_ERR;
                            }
                            else if(cmp->type==FUNCTION)
                            {
                              fcmpd=cmp->data;
                              if(get_type(fcmpd->ret_par_types,0)!=*expt)return EXPRESSION_ERR;
                            }
                          }
                        }
                        else
                        {
                          vcmpd=cmp->data;
                          if((vcmpd->type)!=*expt)return EXPRESSION_ERR;
                        }
                      }
                      break;
                default: break;
              }
              break;
    case MAIN_BODY:
              if(Ac->was_func==true)
              {
                htab_clear(lsymtab);
                if(Ac->is_ret_err==true)return OTHER_SEM_ERR;
                Ac->was_func=false;
              }
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(Ac->act_varID,token->mem);
                      cmp=htab_search(gsymtab,Ac->act_varID);
                      if(cmp==NULL)return SEMANTIC_ERR;
                      else
                      {
                        if(cmp->type==FUNCTION)return SEMANTIC_ERR;
                        else
                        {
                          vcmpd=cmp->data;
                          if(Ac->is_write==true && vcmpd->is_def==false)return SEMANTIC_ERR;
                          if(Ac->is_readln==true)
                          {
                            if(vcmpd->type==tBOOLEAN)return EXPRESSION_ERR;
                            vcmpd->is_def=true;
                          }
                        }
                      }
                      break;
                case KwWrite:
                      Ac->is_write=true;
                      break;
                case KwReadln:
                      Ac->is_readln=true;
                      break;
                case OpPZat:
                      if(Ac->is_write==true)Ac->is_write=false;
                      if(Ac->is_readln==true)Ac->is_readln=false;
                      break;
                case OpPrir:
                      cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                      if(cmp->type==IDENTIFIER)
                      {
                        vcmpd=cmp->data;
                        vcmpd->is_def=true;
                      }
                      break;
                case OpKonec:
                      if(*expt!=tERR)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                        vcmpd=cmp->data;
                        if(vcmpd->type!=*expt)return EXPRESSION_ERR;
                      }
                      break;
                case KwEnd:
                      if(*expt!=tERR)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                        vcmpd=cmp->data;
                        if(vcmpd->type!=*expt)return EXPRESSION_ERR;
                      }
                      break;
                default: break;
              }
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

void free_all(T_ParserItem **p, Stack st, int stack_erase, int token_mem_free, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, t_varfunc_list *vflistp, t_lablist *lablistp, tListOfInstr *inslistp, int l_dispose)
{
  Hitem *cmp=NULL;
  T_FuncData *fcmpd=NULL;
  t_list_item *tmp=NULL;
  PItems_free(&p);
  if(token_mem_free==1)free(token->mem);
  if(stack_erase)S_erase(&st);
  htab_free(lsymtab);
  if(vflistp!=NULL)
  {
    tmp=vflistp->First;
    while(tmp!=NULL)
    {
      if(tmp->type==FUNCTION)
      {
        cmp=htab_search(gsymtab,tmp->item_ID);
        fcmpd=cmp->data;
        free(fcmpd->ret_par_types);
      }
      tmp=tmp->next;
    }
  }
  htab_free(gsymtab);
  free(Ac->act_funcID);
  free(Ac->act_varID);
  free(Ac->act_rptypes);
  free(Ac);
  if(l_dispose==1)
  {
    varfuncL_dispose(vflistp);
    free(vflistp);
    labL_dispose(lablistp);
    free(lablistp);
    listFree(inslistp);
    free(inslistp);
  }
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
