#include "struct.h"
#include "generator.h"
#include "error.h"

#include "frame.h"
#include "stack.h"
#include "list.h"

void frameInit(tListofVariables *F)
{
  F->first  = NULL;
  F->last   = NULL;
  F->active = NULL;
}
  
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
				var->inic=false;
				frameInsert(F, var);
			}
			L->Active=L->Active->next;
		}
		return F;
	}
	else return NULL;
}

			

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
				if(strcmp(L->Active->item_ID, item_ID)==0)
					{
						struct FrameVariable *var;
						var=malloc(sizeof(struct FrameVariable));
						var->type=L->Active->dattype;
						var->name=L->Active->item_ID;
						var->param=false;
						var->inic=false;
						frameInsert(F, var);
						
						if(L->Active->flist != NULL)
						{
							L->Active->flist->Active=L->Active->flist->First;
							while(L->Active->flist->Active != NULL)
							{
								var=malloc(sizeof(struct FrameVariable));
								var->type=L->Active->flist->Active->dattype;
								var->name=L->Active->flist->Active->item_ID;
								var->param=L->Active->flist->Active->is_param;
								var->inic=false;
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
	printf("premenna vo frame sa nenasla\n");
	return NULL;
}

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
	printf("premenna dest = %s sa nenasla vo frame\n",s);
	return NULL;
}
