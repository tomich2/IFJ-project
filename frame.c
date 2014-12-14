// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#include "struct.h"
#include "generator.h"
#include "error.h"

#include "frame.h"
#include "stack.h"
#include "list.h"

/*inicializacia zoznamu*/
void frameInit(tListofVariables *F)
{
  F->first  = NULL;
  F->last   = NULL;
  F->active = NULL;
}
  
/*uvolnenie zoznamu premennych*/
void frameFree(tListofVariables *F)
{
  struct FrameVariable *var;
  if(F != NULL)
  {
	  while (F->first != NULL)
	  {
		var= F->first;
		F->first = F->first->nextvar;
		free(var);
	  }
	  free(F);
  }
}

/*vlozenie premennej do zoznamu*/
void frameInsert(tListofVariables *F, struct FrameVariable *var)
{
  
  var->data.s=NULL;
  var->nextvar = NULL;
  if (F->first == NULL)
     F->first = var;
  else
     F->last->nextvar=var;
  F->last=var;
}

/*vytvorenie globalneho zoznamu premennych*/
tListofVariables* createGlobFrame(t_varfunc_list *L)
{
	if(L != NULL)
	{
		tListofVariables *F;
		F=malloc(sizeof(struct tListofVariables));
		frameInit(F);
		L->Active=L->First;
		while(L->Active != NULL)
		{
			if(L->Active->type==IDENTIFIER)
			{
				struct FrameVariable *var;
				var=malloc(sizeof(struct FrameVariable));
				var->type=L->Active->dattype;
				var->name=L->Active->item_ID;
				var->param=false;

				frameInsert(F, var);
			}
			L->Active=L->Active->next;
		}
		return F;
	}
	else return NULL;
}

			
/*vytvorenie lokalneho zoznamu premennych podla mena funkcie*/
tListofVariables* createFrame(char *item_ID, t_varfunc_list *L)
{
	if(L != NULL)
	{
		tListofVariables *F;
		F=malloc(sizeof(struct tListofVariables));
		frameInit(F);
		L->Active=L->First;
		while(L->Active != NULL)
		{
			if(L->Active->type == FUNCTION)
			{
				if(strcmp(L->Active->item_ID, item_ID)==0)	//najde meno funkcie
					{
						struct FrameVariable *var;
						var=malloc(sizeof(struct FrameVariable));
						var->type=L->Active->dattype;
						var->name=L->Active->item_ID;
						var->param=false;
						
						frameInsert(F, var);
						
						if(L->Active->flist != NULL)
						{
							L->Active->flist->Active=L->Active->flist->First;
							while(L->Active->flist->Active != NULL)		//do zoznamu vlozi vsetky premenne v danej funkcii
							{
								var=malloc(sizeof(struct FrameVariable));
								var->type=L->Active->flist->Active->dattype;
								var->name=L->Active->flist->Active->item_ID;
								var->param=L->Active->flist->Active->is_param;
								
								frameInsert(F, var);
								L->Active->flist->Active=L->Active->flist->Active->next;
							}
						}
								
					}
			}
			L->Active=L->Active->next;
		}
		return F;
	}
	else return NULL;
}

/*najde adresu premennej v globalnom alebo lokalnom zozname*/
struct FrameVariable* findFrameVar(struct Variable *a, tListofVariables* globalFrame, tListofVariables* localFrame)
{
	if(localFrame != NULL)
	{
		localFrame->active=localFrame->first;
		while(localFrame->active != NULL)
		{
			if(strcmp(a->data.s,localFrame->active->name)==0)
			{
				return localFrame->active;
			}
			localFrame->active=localFrame->active->nextvar;
		}
	}
	if(globalFrame != NULL)
	{
		globalFrame->active=globalFrame->first;
		while(globalFrame->active != NULL)
		{
			if(strcmp(a->data.s,globalFrame->active->name)==0)
			{
				return globalFrame->active;
			}
			globalFrame->active=globalFrame->active->nextvar;
		}
	}
	
	return NULL;
}

/*najde adresu premennej v zoznamoch*/
struct FrameVariable* findFrameDest(char *s, tListofVariables* globalFrame, tListofVariables* localFrame)
{
	if (localFrame != NULL)
	{
		localFrame->active=localFrame->first;
		while(localFrame->active != NULL)
		{
			
			if(strcmp(s,localFrame->active->name)==0)
			{
				return localFrame->active;
			}
			localFrame->active=localFrame->active->nextvar;
		}
	}
	if (globalFrame != NULL)
	{
		globalFrame->active=globalFrame->first;
		while(globalFrame->active != NULL)
		{
			if(strcmp(s,globalFrame->active->name)==0)
			{
				return globalFrame->active;
			}
			globalFrame->active=globalFrame->active->nextvar;
		}
	}
	
	return NULL;
}
