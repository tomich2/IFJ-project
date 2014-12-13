// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#ifndef _FRAME_H
#define _FRAME_H

#include "struct.h"
#include "generator.h"
#include "list.h"
#include <stdbool.h>

/*struktura premennej pre interpret*/
typedef struct FrameVariable{

	VariableData data;
	T_vartype type;
	char *name;
	struct FrameVariable *nextvar;	
	bool param;
}tFrameVariable;



typedef struct tListofVariables
{
  tFrameVariable *first;  
  tFrameVariable *last;   
  tFrameVariable *active;
} tListofVariables;

void frameInit(tListofVariables *F);
void frameFree(tListofVariables *F);
void frameInsert(tListofVariables *F, struct FrameVariable *var);
tListofVariables* createGlobFrame(t_varfunc_list *L);
tListofVariables* createFrame(char *item_ID, t_varfunc_list *L);
struct FrameVariable* findFrameVar(struct Variable *a, tListofVariables* globalFrame, tListofVariables* localFrame);
struct FrameVariable* findFrameDest(char *s, tListofVariables* globalFrame, tListofVariables* localFrame);





#endif
