// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"
#include "rules.h"
#include "stack.h"
#include "error.h"
#include "expr.h"


// funkcia vykonava syntakticku analyzu a vracia typ chyby alebo uspech
ERROR_MSG top_down()
{
  // vzdy, ked nastane chyba, ulozi sa do premennej err jej typ, uvolni sa pamat pomocou funkcie free_all a funkcia top_down sa ukonci s navratovou hodnotou chyby
  ERROR_MSG err=EVERYTHINGSOKAY; // premenna pre chybove stavy

  err=get_token(); // nacita prvy token

  if(err!=EVERYTHINGSOKAY) // lexikalna chyba
  {
    if(token->mem!=NULL)free(token->mem);
    return err;
  }

  if(token->identity==EndOfFile)return SYNTAX_ERR; // syntakticka chyba=prazdny subor

  T_ParserItem **PItems=NULL; // ukazovatel na ukazovatel terminalu alebo neterminalu, v podstate "pole" terminalov a neterminalov ulozenych v strukture
  err=PItems_alloc(&PItems);
  if(err)return err;

  T_ParserItem *PItem_top; // prvok na vrchole zasobniku

  int i=0; // pocitadlo prvkov ukladanych na zasobnik

  T_vartype expr_type=tERR; // typ vyrazu, posiela sa odkazom do funkcie ExprParse, ktora ho zmeni na aktualny typ vyrazu

  T_State state; // urcuje, ktora cast programu sa analyzuje(deklaracia premennych, definicia funkcie...)
  bool is_func; // pomocna premenna pre zistenie, ci sa analyzuje cast funkcie

  Stack p_stack; // zasobnik pre syntakticku analyzu
  init(&p_stack,sizeof(T_ParserItem));
  Stack s_stack; // zasobnik pre navestia
  init(&s_stack,sizeof(int*));
  Stack ifbeg_stack; // zasobnik pre pocitadlo zlozenych prikazov vnutri prikazu if
  init(&ifbeg_stack,sizeof(int*));
  Stack whbeg_stack; // zasobnik pre pocitadlo zlozenych prikazov vnutri prikazu while
  init(&whbeg_stack,sizeof(int*));

  htab_t *glob_sym_table=htab_init(GTAB_SIZE); // inicializacia globalnej tabulky symbolov
  htab_t *loc_sym_table=htab_init(GTAB_SIZE); // inicializacia lokalnej tabulky symbolov

  // vlozenie unikatnych docasnych premennych do globalnej tabulky symbolov
  err=htab_new(glob_sym_table,TMPU,IDENTIFIER,NULL,0);
  err=htab_new(glob_sym_table,TMPU2,IDENTIFIER,NULL,0);
  err=htab_new(glob_sym_table,TMParam,IDENTIFIER,NULL,0);
  err=htab_new(glob_sym_table,TMFunc,IDENTIFIER,NULL,0);

  T_Actual *Act=calloc(1,sizeof(*Act)); // pomocna struktura pre semantiku
  // inicializacia hodnot pomocnej struktury Act
  Act->act_funcID=NULL;
  Act->act_varID=NULL;
  Act->act_rptypes=calloc(1,sizeof(char)*MAX_RPTYPES);
  Act->was_func=false;
  Act->is_write=false;
  Act->is_readln=false;
  Act->is_ret_err=true;
  Act->is_else=false;
  Act->is_while=false;
  Act->rpt_size=2;
  Act->begincnt=0;
  Act->ifbegcnt=0;
  Act->whbegcnt=0;
  Act->labIDcnt=1;
  Act->flist=NULL;

  size_t tmem_size; // premenna pre aktualnu velkost token->mem

  // inicializacia zoznamov
  t_varfunc_list *vflist=malloc(sizeof(*vflist)); // zoznam premennych a funkcii
  varfuncL_init(vflist);
  t_lablist *lablist=malloc(sizeof(*lablist)); // zoznam navesti
  labL_init(lablist);
  tListOfInstr *inslist=malloc(sizeof(*inslist)); // zoznam instrukcii
  listInit(inslist);

  const char TMPUV[]=TMPU; // unikatna docasna premenna

  if(token->mem!=NULL) // konvertovanie token->mem na UPPERCASE
  {
    if(token->identity!=DtString)strtoupper(&token->mem);
    tmem_size=strlen(token->mem)+1;
  }


  if(get_rule(START,PItems,&state,&is_func)) // podla pravidla vykona expanziu a pravu stranu pravidla ulozi do PItems
  {
    free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist); // ak je prvy token nespravny=syntakticka chyba
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
      free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
      return SYNTAX_ERR;
    }

    PItem_top=top(&p_stack); // ulozi sa vrchol zasobnika

    if(PItem_top->type==NONTERMINAL) // na vrchole zasobnika je neterminal
    {

      if(PItem_top->value.nonterm.type==EXPR) // ak je na vrchole zasobnika neterminal EXPR(vyraz), spracuje ho precedencna analyza
      {
        err=ExprParse(glob_sym_table,loc_sym_table,&expr_type,inslist);
        if(err) // chybny vyraz=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
          return err;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
      }
      else // pre normalne neterminaly vykona expanziu podla pravidla, pravu stranu ulozi do PItems a nasledne na zasobnik
      {
        if(get_rule(PItem_top->value.nonterm.type,PItems,&state,&is_func)) // neexistuje pravidlo=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
          return SYNTAX_ERR;
        }
        pop(&p_stack); //  expandovany neterminal sa odstrani zo zasobnika
        PItem_top=NULL;
        while(PItems[i]->type!=EMPTY) // ulozi PItems na zasobnik
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
          // syntakticka analyza presla v aktualnom kroku uspesne, vola sa semanticka analyza
          err=semantic(&state,glob_sym_table,loc_sym_table,Act,&expr_type,tmem_size,vflist,lablist,inslist,TMPUV,&s_stack,&ifbeg_stack,&whbeg_stack);
          if(err!=EVERYTHINGSOKAY)
          {
            free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
            return err;
          }
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // token nie je literal=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
          return SYNTAX_ERR;
        }
      }
      else
      {
        if(token->identity==PItem_top->value.term.type) // ak sa typ terminalu zhoduje s tokenom, odstrani sa terminal zo zasobniku a pokracuje sa na dalsi token
        {
          // syntakticka analyza presla v aktualnom kroku uspesne, vola sa semanticka analyza
          err=semantic(&state,glob_sym_table,loc_sym_table,Act,&expr_type,tmem_size,vflist,lablist,inslist,TMPUV,&s_stack,&ifbeg_stack,&whbeg_stack);
          if(err!=EVERYTHINGSOKAY)
          {
            free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
            return err;
          }
          if(Act->begincnt==0 && state==FUNC_BODY)is_func=false;
          pop(&p_stack);
          PItem_top=NULL;
        }
        else // terminal a token sa nezhoduju=syntakticka chyba
        {
          free_all(PItems,p_stack,1,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
          if(token->identity==KwSort || token->identity==KwFind)return SEMANTIC_ERR;
          return SYNTAX_ERR;
        }
      }
    //fprintf(stderr,"debug..token: %s state: %d\n", token->mem,state);
      free(token->mem);
      token->mem=NULL;
      expr_type=tERR; // typ vyrazu sa vzdy nastavi na tERR, podla toho sa zisti, ci prebehla precedencna analyza, ak ano, premenna nebude tERR

      err=get_token(); // nacita dalsi token
      if(err!=EVERYTHINGSOKAY)
      {
        free_all(PItems,p_stack,1,0,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
        return err;
      }

      if(token->mem!=NULL) // konvertovanie token->mem na UPPERCASE
      {
        if(token->identity!=DtString)strtoupper(&token->mem);
        tmem_size=strlen(token->mem)+1;
      }
    }
  }
  if(token->identity!=EndOfFile) // zdrojovy subor obsahuje nejake znaky po ukoncujucej bodke=syntakticka chyba
  {
   free_all(PItems,p_stack,0,1,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
   return SYNTAX_ERR;
  }
   //showList(inslist);
  interpretLoop(inslist,vflist,lablist); // funkcia interpretu

  free_all(PItems,p_stack,0,0,glob_sym_table,loc_sym_table,Act,vflist,lablist,inslist);
  return EVERYTHINGSOKAY;
}

// funkcia semantickej analyzy, vykonava aj generovanie instrukcii
ERROR_MSG semantic(T_State *st, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, T_vartype *expt, size_t tmems, t_varfunc_list *vflistp,
t_lablist *lablistp, tListOfInstr *inslistp, const char *TMPUV, Stack *s_stack, Stack *ib_stack, Stack *wb_stack)
{
  // inicializacia pomocnych premennych
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
  t_func_list *flist_tmp=NULL;

  if(Ac->rpt_size==MAX_RPTYPES) // ked velkost retazca typov parametrov dosiahne urcene maximum, alokuje sa viac pamate
  {
  size_t s=sizeof(Ac->act_rptypes);
  Ac->act_rptypes=realloc(Ac->act_rptypes,s+10);
  }

  switch(*st) // podla stavu, v ktorom sa analyza nachadza, sa vyberie vhodna semanticka akcia
  {
    // deklaracia glob. premennych, aktualny identifikator premennej sa vzdy ulozi do pomocnej struktury
    // pri type sa premenna vklada do globalnej tabulky symbolov, ak uz tabulka obsahuje premennu s rovnakym ID=semanticka chyba
    // premenne sa tiez vkladaju do zoznamu premennych a funkcii
    case GLOBVAR_DEK:
              vdattmp=calloc(1,sizeof(T_VarData));
              switch(token->identity) // podla identity aktualneho tokenu sa vyberie akcia
              {
                case ID:
                      if(strcmp(token->mem,"COPY")==0 || strcmp(token->mem,"LENGTH")==0) // premenne sa nesmu volat ako vstavane funkcie
                      {
                        free(vdattmp);
                        return SEMANTIC_ERR;
                      }
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
                case KwFind: // premenne sa nesmu volat ako vstavane funkcie
                  case KwSort:
                      free(vdattmp);
                      return SEMANTIC_ERR;
                default: break;
              }
              free(vdattmp);
              break;

    // identifikator funkcie sa ulozi do pomocnej struktury
    case FUNC_ID:
              if(token->identity==OpLZat || token->identity==KwFunction)break; // tieto tokeny sa preskocia
              if(strcmp(token->mem,"COPY")==0 || strcmp(token->mem,"LENGTH")==0 ||
              token->identity==KwFind || token->identity==KwSort)return SEMANTIC_ERR; // funkcia sa nesmie volat ako vstavane funkcie
              if(Ac->was_func==true) // ak sa uz analyzovala ina funkcia, vyprazdni sa lokalna tabulka symbolov
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
              Ac->flist=malloc(sizeof(*(Ac->flist))); // zoznam parametrov a lokalnych premennych jednotlivych funkcii
              funcL_init(Ac->flist);
              break;

    // parametre funkcie sa ukladaju do lokalnej tabulky symbolov aj do zoznamu parametrov a lokalnych premennych aktualnej funkcie
    // zaciatocne pismeno typu parametru sa konkatenuje do retazca act_rptypes
    // princip je totozny ako pri glob. premennych
    case FUNC_PARAMS:
              vdattmp=malloc(sizeof(*vdattmp));
              if(vdattmp==NULL)return INTERN_INTERPRETATION_ERR;
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      if(strcmp(token->mem,"COPY")==0 || strcmp(token->mem,"LENGTH")==0)
                      {
                        free(vdattmp);
                        return SEMANTIC_ERR;
                      }
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tINTEGER,true)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tSTRING,true)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tREAL,true)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tBOOLEAN,true)!=0)return INTERN_INTERPRETATION_ERR;
                      Ac->rpt_size++;
                      strcat(Ac->act_rptypes,"b");
                      break;
                case KwFind:
                  case KwSort:
                      free(vdattmp);
                      return SEMANTIC_ERR;
                default: break;
              }
              free(vdattmp);
              break;

    // zisti sa navratovy typ funkcie, na zaciatok retazca act_rptypes sa ulozi prve pismeno tohto typu
    // udaje o funkcii sa ulozia do globalnej tabulky symbolov, ak sa najde rovnake ID=semanticka chyba
    // ID a typ sa tiez ulozi do zoznamu premennych a funkcii
    case FUNC_TYPE:
              if(token->identity==OpDek || token->identity==OpPZat || token->identity==OpKonec)return EVERYTHINGSOKAY; // tieto tokeny sa preskocia
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
                case KwForward: // vyhlada sa aktualna funkcia v globalnej tab. sym., nastavi sa priznak, ze nebola definovana
                      cmp=htab_search(gsymtab,Ac->act_funcID);
                      fcmpd=cmp->data;
                      fcmpd->is_def=false;
                      Ac->is_ret_err=false;
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
                if(cmp->type==IDENTIFIER)
                {
                  free(fdattmp->ret_par_types);
                  free(fdattmp);
                  return SEMANTIC_ERR;
                }
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
                    if(funcL_compare(Ac->flist,varfuncL_getflist(vflistp,Ac->act_funcID))!=0)
                    {
                      funcL_dispose(Ac->flist);
                      free(Ac->flist);
                      free(fdattmp->ret_par_types);
                      free(fdattmp);
                      return SEMANTIC_ERR;
                    }
                    flist_tmp=varfuncL_getflist(vflistp,Ac->act_funcID);
                    if(flist_tmp!=Ac->flist)
                    {
                      funcL_dispose(Ac->flist);
                      free(Ac->flist);
                      Ac->flist=flist_tmp;
                    }
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
                if(varfuncL_insertlast(vflistp,Ac->flist,FUNCTION,Ac->act_funcID,ftype)!=0)return INTERN_INTERPRETATION_ERR;
                free(fdattmp);
              }
              break;

    // deklaracia lokalnych premennych je analogicka s deklaraciou globalnych, ale lokalne premenne sa ukladaju do lokalnej tabulky symbolov
    // lokalne premenne sa ukladaju aj do zoznamu parametrov a lokalnych premennych aktualnej funkcie
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
                      if(strcmp(token->mem,"COPY")==0 || strcmp(token->mem,"LENGTH")==0)
                      {
                        free(vdattmp);
                        return SEMANTIC_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tINTEGER,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tSTRING,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tREAL,false)!=0)return INTERN_INTERPRETATION_ERR;
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
                      if(funcL_insertfirst(Ac->flist,Ac->act_varID,tBOOLEAN,false)!=0)return INTERN_INTERPRETATION_ERR;
                      break;
                case KwFind:
                  case KwSort:
                      free(vdattmp);
                      return SEMANTIC_ERR;
                default: break;
              }
              free(vdattmp);
              break;

    // v tele funkcie sa kontroluje, ci boli deklarovane vsetky premenne, kontroluje sa spravny typ vyrazu pri priradeni a generuju sa potrebne instrukcie
    case FUNC_BODY: ;
              int lab;
              int lab2;
              switch(token->identity)
              {
                case ID:
                      Ac->act_varID=realloc(Ac->act_varID,tmems);
                      if(Ac->act_varID==NULL)return INTERN_INTERPRETATION_ERR;
                      strcpy(Ac->act_varID,token->mem);
                      cmp=(Hitem*)htab_search(lsymtab,token->mem); // hlada sa identifikator, najprv v lokalnej tabulke
                      if(cmp==NULL)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,token->mem);
                        if(cmp==NULL)return SEMANTIC_ERR; // ak sa ID nenajde v tabulkach, je nedeklarovany
                        else // identifikator sa nasiel v globalnej tabulke
                        {
                          if(cmp->type==FUNCTION)
                          {
                            if(strcmp(cmp->key,Ac->act_funcID)!=0)return SEMANTIC_ERR;
                            fcmpd=cmp->data;
                            if(Ac->is_write==true)
                            {
                              if(fcmpd->is_ret==false)return SEMANTIC_ERR;
                              varA=malloc(sizeof(*varA));
                              if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                              varA->data.s=malloc(strlen(Ac->act_varID)+1);
                              if(varA->data.s==NULL)
                              {
                                free(varA);
                                return INTERN_INTERPRETATION_ERR;
                              }
                              strcpy(varA->data.s,Ac->act_varID);
                              varA->type=tVAR;
                              generator(inslistp,I_PRINT,varA,NULL,NULL);
                            }
                            else if(Ac->is_readln==true)
                            {
                              if(get_type(fcmpd->ret_par_types,0)==tBOOLEAN)return EXPRESSION_ERR;
                              fcmpd->is_ret=true;
                              Ac->is_ret_err=false;
                              generator(inslistp,I_READ,NULL,NULL,Ac->act_varID);
                            }
                          }
                          else
                          {
                            vcmpd=cmp->data;
                            if(Ac->is_write==true )
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
                              strcpy(varA->data.s,Ac->act_varID);
                              varA->type=tVAR;
                              generator(inslistp,I_PRINT,varA,NULL,NULL);
                            }
                            if(Ac->is_readln==true)
                            {
                              if(vcmpd->type==tBOOLEAN)return EXPRESSION_ERR;
                              vcmpd->is_def=true;
                              generator(inslistp,I_READ,NULL,NULL,Ac->act_varID);
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
                          strcpy(varA->data.s,Ac->act_varID);
                          varA->type=tVAR;
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
                      cmp=(Hitem*)htab_search(lsymtab,Ac->act_varID);
                      if(cmp==NULL)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                        if(cmp->type==IDENTIFIER)
                        {
                          vcmpd=cmp->data;
                          vcmpd->is_def=true;
                        }
                        else if(cmp->type==FUNCTION)
                        {
                          fcmpd=cmp->data;
                          fcmpd->is_ret=true;
                          Ac->is_ret_err=false;
                        }
                      }
                      else
                      {
                        vcmpd=cmp->data;
                        vcmpd->is_def=true;
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
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(TMPLEN*sizeof(char));
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,TMPUV);
                        varA->type=tVAR;
                        generator(inslistp,I_ASSIGN,varA,NULL,Ac->act_varID);
                      }
                      break;
                case KwBegin:
                      if(Ac->begincnt==0)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=0;
                        varA->data.s=NULL;
                        generator(inslistp,I_GOTO,varA,NULL,NULL);
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=Ac->labIDcnt;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                        err=labL_insertlast(lablistp,ins_adress,Ac->labIDcnt,Ac->act_funcID);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                        Ac->labIDcnt++;
                      }
                      Ac->begincnt++;
                      if(Ac->is_else==true)Ac->ifbegcnt++;
                      if(Ac->is_while==true)Ac->whbegcnt++;
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
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(TMPLEN*sizeof(char));
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,TMPUV);
                        varA->type=tVAR;
                        generator(inslistp,I_ASSIGN,varA,NULL,Ac->act_varID);
                      }
                      if(Ac->is_else==true)Ac->ifbegcnt--;
                      if(Ac->is_while==true)Ac->whbegcnt--;
                      if(Ac->ifbegcnt==0 && Ac->is_else==true) // koniec prikazu if
                      {
                        Ac->is_else=false;
                        Ac->ifbegcnt=*((int*)top(ib_stack));
                        pop(ib_stack);
                        // LAB 2
                        lab2=*((int*)top(s_stack));
                        pop(s_stack);
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab2;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                        err=labL_insertlast(lablistp,ins_adress,lab2,NULL);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                      }
                      if(Ac->whbegcnt==0 && Ac->is_while==true) // koniec prikazu while
                      {
                        Ac->is_while=false;
                        Ac->whbegcnt=*((int*)top(wb_stack));
                        pop(wb_stack);
                        // uloz LAB2 zo zasobniku do lab2
                        lab2=*((int*)top(s_stack));
                        pop(s_stack);
                        // uloz LAB1 zo zasobniku do lab
                        lab=*((int*)top(s_stack));
                        pop(s_stack);
                        // GOTO LAB1 nepodmienene
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab;
                        varA->data.s=NULL;
                        generator(inslistp,I_GOTO,varA,NULL,NULL);
                        // vygeneruj LAB2
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab2;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                        err=labL_insertlast(lablistp,ins_adress,lab2,NULL);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                      }
                      if(Ac->begincnt==0)
                      {
                        generator(inslistp,I_RETURN,NULL,NULL,NULL);
                      }
                      break;
                case KwThen:
                      if(*expt!=tBOOLEAN)return EXPRESSION_ERR;
                      // GOTO LAB1 podmienene, uloz LAB1 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      varB=malloc(sizeof(*varB));
                      if(varB==NULL)return INTERN_INTERPRETATION_ERR;
                      varB->data.s=malloc(TMPLEN*sizeof(char));
                      if(varB->data.s==NULL)
                      {
                        free(varB);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(varB->data.s,TMPUV);
                      varB->type=tVAR;
                      generator(inslistp,I_GOTO,varA,varB,NULL);
                      break;
                case KwElse:
                      push(ib_stack,&Ac->ifbegcnt,-1);
                      Ac->ifbegcnt=0;
                      Ac->is_else=true;
                      // uloz LAB1 zo zasobniku do lab
                      lab=*((int*)top(s_stack));
                      pop(s_stack);
                      // GOTO LAB2 nepodmienene, uloz LAB2 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      generator(inslistp,I_GOTO,varA,NULL,NULL);
                      // vygeneruj LAB1
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=lab;
                      ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                      err=labL_insertlast(lablistp,ins_adress,lab,NULL);
                      if(err!=0)return INTERN_INTERPRETATION_ERR;
                      break;
                case KwWhile:
                      Ac->is_while=true;
                      // LAB 1, uloz LAB1 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                      err=labL_insertlast(lablistp,ins_adress,Ac->labIDcnt,NULL);
                      if(err!=0)return INTERN_INTERPRETATION_ERR;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      break;
                case KwDo:
                      if(*expt!=tBOOLEAN)return EXPRESSION_ERR;
                      push(wb_stack,&Ac->whbegcnt,-1);
                      Ac->whbegcnt=0;
                      // GOTO LAB2 podmienene, uloz LAB2 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      varB=malloc(sizeof(*varB));
                      if(varB==NULL)return INTERN_INTERPRETATION_ERR;
                      varB->data.s=malloc(TMPLEN*sizeof(char));
                      if(varB->data.s==NULL)
                      {
                        free(varB);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(varB->data.s,TMPUV);
                      varB->type=tVAR;
                      generator(inslistp,I_GOTO,varA,varB,NULL);
                      break;
                case DtInteger:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tINTEGER;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
                      }
                      break;
                case DtReal:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tREAL;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
                      }
                      break;
                case DtString:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tSTRING;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
                      }
                      break;
                default: break;
              }
              break;

     // v hlavnom tele sa kontroluje, ci boli deklarovane vsetky premenne a funkcie, kontroluje sa spravny typ vyrazu pri priradeni a generuju sa potrebne instrukcie
    case MAIN_BODY:
              if(Ac->was_func==true)
              {
                htab_clear(lsymtab);
                if(Ac->is_ret_err==true)return UNINICIALIZED_ERR; // niektora funkcia nema priradenu navratovu hodnotu
                t_list_item *item=vflistp->First;
                while(item!=NULL) // kontrola, ci su vsetky deklarovane funkcie aj definovane
                {
                  if(item->type==FUNCTION)
                  {
                    cmp=htab_search(gsymtab,item->item_ID);
                    fcmpd=cmp->data;
                    if(fcmpd->is_def==false)return SEMANTIC_ERR;
                  }
                  item=item->next;
                }
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
                            strcpy(varA->data.s,Ac->act_varID);
                            varA->type=tVAR;
                            generator(inslistp,I_PRINT,varA,NULL,NULL);
                          }
                          if(Ac->is_readln==true)
                          {
                            if(vcmpd->type==tBOOLEAN)return EXPRESSION_ERR;
                            vcmpd->is_def=true;
                            generator(inslistp,I_READ,NULL,NULL,Ac->act_varID); // instrukcia pre readln ******
                          }
                        }
                      }
                      break;
                case KwBegin:
                      if(Ac->begincnt==0)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=0;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL); // nulty label=zaciatok hlavneho programu
                        err=labL_insertlast(lablistp,ins_adress,0,NULL);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                        Ac->labIDcnt++;
                      }
                      Ac->begincnt++;
                      if(Ac->is_else==true)Ac->ifbegcnt++;
                      if(Ac->is_while==true)Ac->whbegcnt++;
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
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(TMPLEN*sizeof(char));
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,TMPUV);
                        varA->type=tVAR;
                        generator(inslistp,I_ASSIGN,varA,NULL,Ac->act_varID);
                      }
                      break;
                case KwEnd:
                      if(*expt!=tERR)
                      {
                        cmp=(Hitem*)htab_search(gsymtab,Ac->act_varID);
                        vcmpd=cmp->data;
                        if(vcmpd->type!=*expt)return EXPRESSION_ERR;
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(TMPLEN*sizeof(char));
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,TMPUV);
                        varA->type=tVAR;
                        generator(inslistp,I_ASSIGN,varA,NULL,Ac->act_varID);
                      }
                      if(Ac->is_else==true)Ac->ifbegcnt--;
                      if(Ac->is_while==true)Ac->whbegcnt--;
                      if(Ac->ifbegcnt==0 && Ac->is_else==true) // koniec prikazu if
                      {
                        Ac->is_else=false;
                        Ac->ifbegcnt=*((int*)top(ib_stack));
                        pop(ib_stack);
                        // LAB 2
                        lab=*((int*)top(s_stack));
                        pop(s_stack);
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                        err=labL_insertlast(lablistp,ins_adress,lab,NULL);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                      }
                      if(Ac->whbegcnt==0 && Ac->is_while==true) // koniec prikazu while
                      {
                        Ac->is_while=false;
                        Ac->whbegcnt=*((int*)top(wb_stack));
                        pop(wb_stack);
                        // uloz LAB2 zo zasobniku do lab2
                        lab2=*((int*)top(s_stack));
                        pop(s_stack);
                        // uloz LAB1 zo zasobniku do lab
                        lab=*((int*)top(s_stack));
                        pop(s_stack);
                        // GOTO LAB1 nepodmienene
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab;
                        varA->data.s=NULL;
                        generator(inslistp,I_GOTO,varA,NULL,NULL);
                        // vygeneruj LAB2
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->type=tINTEGER;
                        varA->data.i=lab2;
                        ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                        err=labL_insertlast(lablistp,ins_adress,lab2,NULL);
                        if(err!=0)return INTERN_INTERPRETATION_ERR;
                      }
                      break;
                case KwThen:
                      if(*expt!=tBOOLEAN)return EXPRESSION_ERR;
                      // GOTO LAB1 podmienene, uloz LAB1 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      varB=malloc(sizeof(*varB));
                      if(varB==NULL)return INTERN_INTERPRETATION_ERR;
                      varB->data.s=malloc(TMPLEN*sizeof(char));
                      if(varB->data.s==NULL)
                      {
                        free(varB);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(varB->data.s,TMPUV);
                      varB->type=tVAR;
                      generator(inslistp,I_GOTO,varA,varB,NULL);
                      break;
                case KwElse:
                      push(ib_stack,&Ac->ifbegcnt,-1);
                      Ac->ifbegcnt=0;
                      Ac->is_else=true;
                      // uloz LAB1 zo zasobniku do lab
                      lab=*((int*)top(s_stack));
                      pop(s_stack);
                      // GOTO LAB2 nepodmienene, uloz LAB2 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      generator(inslistp,I_GOTO,varA,NULL,NULL);
                      // vygeneruj LAB1
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=lab;
                      ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                      err=labL_insertlast(lablistp,ins_adress,lab,NULL);
                      if(err!=0)return INTERN_INTERPRETATION_ERR;
                      break;
                case KwWhile:
                      Ac->is_while=true;
                      // LAB 1, uloz LAB1 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      ins_adress=generator(inslistp,I_LABEL,varA,NULL,NULL);
                      err=labL_insertlast(lablistp,ins_adress,Ac->labIDcnt,NULL);
                      if(err!=0)return INTERN_INTERPRETATION_ERR;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      break;
                case KwDo:
                      if(*expt!=tBOOLEAN)return EXPRESSION_ERR;
                      push(wb_stack,&Ac->whbegcnt,-1);
                      Ac->whbegcnt=0;
                      // GOTO LAB2 podmienene, uloz LAB2 na zasobnik
                      varA=malloc(sizeof(*varA));
                      if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                      varA->type=tINTEGER;
                      varA->data.i=Ac->labIDcnt;
                      varA->data.s=NULL;
                      push(s_stack,&Ac->labIDcnt,-1);
                      Ac->labIDcnt++;
                      varB=malloc(sizeof(*varB));
                      if(varB==NULL)return INTERN_INTERPRETATION_ERR;
                      varB->data.s=malloc(TMPLEN*sizeof(char));
                      if(varB->data.s==NULL)
                      {
                        free(varB);
                        return INTERN_INTERPRETATION_ERR;
                      }
                      strcpy(varB->data.s,TMPUV);
                      varB->type=tVAR;
                      generator(inslistp,I_GOTO,varA,varB,NULL);
                      break;
                case DtInteger:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tSTRING;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
                      }
                      break;
                case DtReal:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tSTRING;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
                      }
                      break;
                case DtString:
                      if(Ac->is_write==true)
                      {
                        varA=malloc(sizeof(*varA));
                        if(varA==NULL)return INTERN_INTERPRETATION_ERR;
                        varA->data.s=malloc(strlen(token->mem)+1);
                        if(varA->data.s==NULL)
                        {
                          free(varA);
                          return INTERN_INTERPRETATION_ERR;
                        }
                        strcpy(varA->data.s,token->mem);
                        varA->type=tSTRING;
                        generator(inslistp,I_PRINT,varA,NULL,NULL);
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

// alokuje pamat pre strukturu PItems a jej polozky
// poloziek je 12, pretoze viac terminalov a neterminalov sa nikdy neuklada na zasobnik
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

// uvolni strukturu PItems a jej polozky
void PItems_free(T_ParserItem ***Ptr)
{
  for(int i=0;i<=11;i++)
  {
    free((*Ptr)[i]);
  }
  free(*Ptr);
}

// uvolni alokovanu pamat, daju sa nastavit priznaky pre vymazanie zasobnika a uvolnenie token->mem
void free_all(T_ParserItem **p, Stack st, int stack_erase, int token_mem_free, htab_t *gsymtab, htab_t *lsymtab, T_Actual *Ac, t_varfunc_list *vflistp, t_lablist *lablistp, tListOfInstr *inslistp)
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
  varfuncL_dispose(vflistp);
  free(vflistp);
  labL_dispose(lablistp);
  free(lablistp);
  listFree(inslistp);
  free(inslistp);
}

// vrati navratovy typ funkcie alebo typ parametru
// str je retazec pismen, ktore oznacuju jednotlive typy
// pos urcuje, coho typ sa vrati, 0=navratovy typ funkcie, 1=prvy parameter 2=druhy parameter ...
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

void strtoupper(char **string)
{
  int len=strlen(*string);
  for(int i=0;i<=len;i++)
  {
    *((*string)+i)=toupper(*((*string)+i));
  }
}
