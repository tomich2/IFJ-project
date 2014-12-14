// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef LIST_H_
#define LIST_H_
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ial.h"
#include "generator.h"


typedef struct fitem{
        struct fitem *next;
        T_vartype dattype;
        bool is_param;
        char *item_ID;
        }t_flist_item;

typedef struct{
        t_flist_item *First;
        t_flist_item *Active;
        }t_func_list;

typedef struct item{
        struct item *next;
        t_func_list *flist; // ukazovatel na zoznam parametrov a lokalnych premennych
        types type; // funkcia alebo premenna
        T_vartype dattype; // datovy typ
        char *item_ID;
        }t_list_item;

typedef struct{
        t_list_item *First;
        t_list_item *Last;
        t_list_item *Active;
        }t_varfunc_list;

typedef struct labitem{
        struct labitem *next;
        tListItem *ins_ptr;
        int lab_ID;
        char *func_name;
        }t_lablist_item;

typedef struct{
        t_lablist_item *First;
        t_lablist_item *Last;
        t_lablist_item *Active;
        }t_lablist;

void varfuncL_init(t_varfunc_list *l);
int varfuncL_insertlast(t_varfunc_list *l, t_func_list *flistp, types typep, char *idp, T_vartype dtypep);
void varfuncL_dispose(t_varfunc_list *l);

void funcL_init(t_func_list *l);
int funcL_insertfirst(t_func_list *l, char *idp, T_vartype dtypep, bool is_parp);
void varfuncL_deletelast(t_varfunc_list *l);
void funcL_dispose(t_func_list *l);

void labL_init(t_lablist *l);
int labL_insertlast(t_lablist *l, tListItem *insptr, int labID, char *func);
void labL_dispose(t_lablist *l);
#endif
