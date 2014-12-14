// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void varfuncL_init(t_varfunc_list *l)
{
  l->First=NULL;
  l->Last=NULL;
  l->Active=NULL;
}

int varfuncL_insertlast(t_varfunc_list *l, t_func_list *flistp, types typep, char *idp, T_vartype dtypep)
{
  t_list_item *new=malloc(sizeof(*new));
  if(new==NULL)return 1;
  new->flist=flistp;
  new->type=typep;
  new->item_ID=malloc(strlen(idp)+1);
  if(new->item_ID==NULL)
  {
    free(new);
    return 1;
  }
  else strcpy(new->item_ID,idp);
  new->dattype=dtypep;
  if(l->First==NULL)
  {
    new->next=NULL;
    l->First=new;
    l->Last=new;
  }
  else
  {
    l->Last->next=new;
    l->Last=new;
    new->next=NULL;
  }
  return 0;
}

t_func_list* varfuncL_getflist(t_varfunc_list *l, char *func)
{
  if(l->First==NULL)return NULL;
  t_list_item *tmp;
  tmp=l->First;
  while(tmp!=NULL)
  {
    if(strcmp(tmp->item_ID,func)==0)return tmp->flist;
    else tmp=tmp->next;
  }
  return NULL;
}
void varfuncL_dispose(t_varfunc_list *l)
{
  if(l==NULL)return;
  t_list_item *tmp, *tmp2;
  tmp=l->First;
  while(tmp!=NULL)
  {
    tmp2=tmp;
    tmp=tmp->next;
    if(tmp2->type==FUNCTION)
    {
      funcL_dispose(tmp2->flist);
      free(tmp2->flist);
    }
    free(tmp2->item_ID);
    free(tmp2);
  }
}

void funcL_init(t_func_list *l)
{
  l->First=NULL;
  l->Active=NULL;
}

int funcL_insertfirst(t_func_list *l, char *idp, T_vartype dtypep, bool is_parp)
{
  t_flist_item *new=malloc(sizeof(*new));
  if(new==NULL)return 1;
  new->item_ID=malloc(strlen(idp)+1);
  if(new->item_ID==NULL)
  {
    free(new);
    return 1;
  }
  strcpy(new->item_ID,idp);
  new->dattype=dtypep;
  new->is_param=is_parp;
  if(l->First==NULL)
  {
    new->next=NULL;
    l->First=new;
  }
  else
  {
    new->next=l->First;
    l->First=new;
  }
  return 0;
}

int funcL_compare(t_func_list *l1, t_func_list *l2)
{
  if(l1==NULL || l2==NULL)return 1;
  if(l1->First==NULL && l2->First==NULL)return 0;
  t_flist_item *tmp1, *tmp2;
  tmp1=l1->First;
  tmp2=l2->First;
  while(tmp1!=NULL && tmp2!=NULL)
  {
    if(strcmp(tmp1->item_ID,tmp2->item_ID)!=0)return 1;
    else
    {
      tmp1=tmp1->next;
      tmp2=tmp2->next;
    }
  }
  if(tmp1!=NULL || tmp2!=NULL)return 1;
  else return 0;
}

void funcL_dispose(t_func_list *l)
{
  if(l==NULL)return;
  t_flist_item *tmp, *tmp2;
  tmp=l->First;
  while(tmp!=NULL)
  {
    tmp2=tmp;
    tmp=tmp->next;
    free(tmp2->item_ID);
    free(tmp2);
  }
}

void labL_init(t_lablist *l)
{
  l->First=NULL;
  l->Last=NULL;
  l->Active=NULL;
}

int labL_insertlast(t_lablist *l, tListItem *insptr, int labID, char *func)
{
  t_lablist_item *new=malloc(sizeof(*new));
  if(new==NULL)return 1;
  new->ins_ptr=insptr;
  new->lab_ID=labID;
  if(func!=NULL)
  {
    new->func_name=malloc(strlen(func)+1);
    if(new->func_name==NULL)
    {
      free(new);
      return 1;
    }
    strcpy(new->func_name,func);
  }
  else new->func_name=NULL;
  if(l->First==NULL)
  {
    new->next=NULL;
    l->First=new;
    l->Last=new;
  }
  else
  {
    l->Last->next=new;
    l->Last=new;
    new->next=NULL;
  }
  return 0;
}

void labL_dispose(t_lablist *l)
{
  if(l==NULL)return;
  t_lablist_item *tmp, *tmp2;
  tmp=l->First;
  while(tmp!=NULL)
  {
    tmp2=tmp;
    tmp=tmp->next;
    if(tmp2->func_name!=NULL)free(tmp2->func_name);
    free(tmp2);
  }
  return;
}
