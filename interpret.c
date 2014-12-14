// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#include "interpret.h"
#include "struct.h"
#include "error.h"
#include "frame.h"
#include "parser.h"
#include "garbage.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define mallConst 10


/*hlavna funkcia interpretu, prechadza listom instrukcii a postupne ich vykonava*/
void interpretLoop(tListOfInstr *instList,t_varfunc_list *varList,t_lablist *lablist)
{
	bool run =true;
	
	int len;

	struct FrameVariable *op1,*op2,*op3;		//operandy instrukcie
	tInstruction *instr;						//instrukcia
	tListofVariables *LocalFrame=NULL;			//Lokalny zoznam premennych
	tListofVariables *GlobalFrame=NULL;			//globalny zoznam premennych
	bool bop1, bop2;
	bop1=false;
	bop2=false;
	bool bcall=false;
	Stack Fstack;								//zasobniky
	init(&Fstack,sizeof(struct tListofVariables*));
	Stack stack;
	init(&stack,sizeof(struct tListofVariables*));
	



	/*pomocne premenne*/

	struct FrameVariable *tmp1;
	struct FrameVariable *tmp2;
	struct FrameVariable *tmparam;
	struct FrameVariable *tmfunc;

	tmp1=malloc(sizeof(struct FrameVariable));
	tmp2=malloc(sizeof(struct FrameVariable));
	tmparam=malloc(sizeof(struct FrameVariable));
	tmfunc=malloc(sizeof(struct FrameVariable));

	tmp1->name=TMPU;
	tmp1->data.s=NULL;
	
	tmp2->name=TMPU2;
	tmp2->data.s=NULL;
	
	tmparam->name=TMParam;
	tmparam->data.s=NULL;
	
	tmfunc->name=TMFunc;
	tmfunc->data.s=NULL;
	


	instList->active=instList->first;		//prva instrukcia sa nastavi ako aktivna


	GlobalFrame=createGlobFrame(varList);		//vytvori sa zoznam globalnych premennych


	while(run)
	{
	instr=instList->active->Instr;

	/*najde sa op1*/
	if(instr->a != NULL)
	{
		if(instr->a->type==tVAR)
		{
			if(strcmp(instr->a->data.s, tmp1->name)==0) op1=tmp1;

			else if(strcmp(instr->a->data.s, tmp2->name)==0) op1=tmp2;

				 else if(strcmp(instr->a->data.s, tmparam->name)==0) op1=tmparam;

					  else if(strcmp(instr->a->data.s, tmfunc->name)==0) op1=tmfunc;

					       else op1=findFrameVar(instr->a, GlobalFrame, LocalFrame);		//funkcia vrati adresu premennej zo zoznamu 
		}
		else
			{
				/*pokial bola zadana konstanta, op1 sa naplni jej hodnotou*/
				op1=malloc(sizeof(struct FrameVariable));
				bop1=true;

				op1->type=instr->a->type;
				

				switch(instr->a->type)
				{
					case tINTEGER:
					{
						op1->data.i=instr->a->data.i;
						break;
					}

					case tREAL:
					{
						op1->data.r=instr->a->data.r;
						break;
					}

					case tBOOLEAN:
					{
						op1->data.b=instr->a->data.b;
						break;
					}
					case tSTRING:
					{
						op1->data.s=instr->a->data.s;
						break;
					}
						default:
						break;
				}

			}
	}
	else op1=NULL;

	/*najde sa op2*/
	if(instr->b != NULL)
	{
		if(instr->b->type==tVAR)
		{
			if(strcmp(instr->b->data.s, tmp1->name)==0) op2=tmp1;

			else if(strcmp(instr->b->data.s, tmp2->name)==0) op2=tmp2;

				 else if(strcmp(instr->b->data.s, tmparam->name)==0) op2=tmparam;

					  else if(strcmp(instr->b->data.s, tmfunc->name)==0) op2=tmfunc;

						   else op2=findFrameVar(instr->b, GlobalFrame, LocalFrame);		//funkcia vrati adresu premennej zo zoznamu 
		}
		else
		{
			/*pokial bola zadana konstanta, op2 sa naplni jej hodnotou*/
			op2=malloc(sizeof(struct FrameVariable));
			bop2=true;

			op2->type=instr->b->type;
			
			
			switch(instr->b->type)
			{

				case tINTEGER:
				{
					op2->data.i=instr->b->data.i;
					break;
				}

				case tREAL:
				{
					op2->data.r=instr->b->data.r;
					break;
				}

				case tBOOLEAN:
				{
					op2->data.b=instr->b->data.b;
					break;
				}

				case tSTRING:
				{
					op2->data.s=instr->b->data.s;
					break;
				}

				default:
				break;
			}

		}
	}
	else op2=NULL;

	/*najde sa op3*/
	if(instr->res != NULL)
	{
		if(strcmp(instr->res, tmp1->name)==0) op3=tmp1;

		else if(strcmp(instr->res, tmp2->name)==0) op3=tmp2;

			 else if(strcmp(instr->res, tmparam->name)==0) op3=tmparam;

				  else if(strcmp(instr->res, tmfunc->name)==0) op3=tmfunc;

					   else op3=findFrameDest(instr->res, GlobalFrame, LocalFrame);		//funkcia vrati adresu premennej zo zoznamu

	}
	else op3=NULL;


	

		switch(instList->active->Instr->Iname)		//rozdelenie instrukcii na zaklade ich typu
		{

			case I_ADD:
			{
				

				if((op1->type)==tINTEGER)
				{
					if((op2->type)==tINTEGER)
					{
		 				op3->data.i=op1->data.i + op2->data.i;
		 				if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tINTEGER;

					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i + op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tREAL;
					}
					else
						{
							Error(4);
							break;
						}

					break;
				}

				else if((op1->type)==tREAL)
					{
						if((op2->type)==tINTEGER)
						{
							op3->data.r=op1->data.r + op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tREAL;
						}

						else if ((op2->type)==tREAL)
						{
							op3->data.r=op1->data.r + op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tREAL;
						}
						else
							{
								Error(4);
								break;
							}

						break;
					}
				else if((op1->type)==tSTRING)
				{
					if(op2->type==tSTRING)		
					{
						/*konkatenacia*/
						op3->data.s=malloc(strlen(op1->data.s)+strlen(op2->data.s)+1);
						InsertFirst(LGar,op3->data.s);
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tSTRING;
						strcpy(op3->data.s,convert_my_string(op1->data.s));
						strcat(op3->data.s,convert_my_string(op2->data.s));
						
					}
					else Error(4);
				}
				
				else {
						Error(4);
						break;
					 }
					 
				
				break;
			}

			case I_SUB:
			{
			

				if((op1->type)==tINTEGER)
				{
					if((op2->type)==tINTEGER)
					{
						op3->data.i=op1->data.i - op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tINTEGER;
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i - op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}
					else
						{
							Error(4);
							break;
						}

					break;
				}

				else if((op1->type)==tREAL)
				{
					if((op2->type)==tINTEGER)
					{
						op3->data.r=op1->data.r - op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.r - op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}
					else
						{
							Error(4);
							break;
						}

					break;
				}
				else {
						Error(4);
						break;
					 }

			
				break;
			}

			case I_MUL:
			{
				
				
					
				if((op1->type)==tINTEGER)
				{
					if((op2->type)==tINTEGER)
					{
						op3->data.i=op1->data.i * op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tINTEGER;
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i * op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}
					else
						{
							Error(4);
							break;
						}

					break;
				}

				else if((op1->type)==tREAL)
				{
					if((op2->type)==tINTEGER)
					{
						op3->data.r=op1->data.r * op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.r * op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}
					else
						{
							Error(4);
							break;
						}

					break;
				}
				else {
						Error(4);
						break;
			     	 }
			
				break;
			}

			case I_DIV:
			{
				
				if((op2->type)==tINTEGER)
				{
					if (op2->data.i == 0)		//delenie nulou
					{
						Error(8);
					}

					if((op1->type)==tINTEGER)
					{
						op3->data.r=(double)op1->data.i / (double)op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					else if ((op1->type)==tREAL)
					{
						op3->data.r=op1->data.r / (double)op2->data.i;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					else
					{
						Error(4);
						break;
					}

					break;
				}

				else if((op2->type)==tREAL)
				{
					if (op2->data.r == 0.0)		//delenie nulou
					{
						Error(8);
						break;
					}

					if((op1->type)==tINTEGER)
					{
						op3->data.r=(double)op1->data.i / op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					else if ((op1->type)==tREAL)
					{
						op3->data.r=op1->data.r / op2->data.r;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
						op3->type=tREAL;
					}

					break;
				}

					else
					{
						Error(4);
						break;
					}

				
				break;
			}

			case I_EQUAL_CMP:
			{
				
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b == op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i == op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tREAL:
							op3->data.b=op1->data.r == op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tSTRING:

		 					if (strcmp (op1->data.s,op2->data.s) == 0)
		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
			 				op3->data.b=false;
			 				if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;
					}

				}

				else{
						op3->data.b=false;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
 	   				}

				
				break;
			}

			case I_NOT_EQUAL_CMP:
			{
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b != op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i != op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tREAL:
							op3->data.b=op1->data.r != op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tSTRING:

		 					if (strcmp (op1->data.s,op2->data.s) != 0)
		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
		 					op3->data.b=false;
		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;
					}
				}

				else{
						op3->data.b=true;
						if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
	   				}
	
				
				break;
			}

			case I_LESS_CMP:
			{
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b < op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tINTEGER:
							op3->data.b=op1->data.i < op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 	 				break;

		 				case tREAL:
							op3->data.b=op1->data.r < op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tSTRING:

		 					if((strlen(op1->data.s)) < (strlen (op2->data.s)))

		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
		 					op3->data.b=false;
		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;
					}
				}

				else{
						Error(4);
						break;
 	   				}
				
				break;
			}

			case I_GR_CMP:
			{
				
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b > op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i > op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
							
			 				break;

		 				case tREAL:
							op3->data.b=op1->data.r > op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tSTRING:

		 					if((strlen(op1->data.s)) > (strlen (op2->data.s)))

		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
		 					op3->data.b=false;
		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;
					}
				}

				else{
						Error(4);
						break;
 	   				}
				
				break;
			}

			case I_LESS_EQUAL_CMP:
			{
				
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b <= op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i <= op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tREAL:
							op3->data.b=op1->data.r <= op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
			 				break;

		 				case tSTRING:

		 					if((strlen(op1->data.s)) <= (strlen (op2->data.s)))

		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
		 					op3->data.b=false;
		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;
					}
				}

				else{
						Error(4);
						break;
 	   				}
				
				break;
			}

			case I_GR_EQUAL_CMP:
			{
				
				if(op1->type == op2->type )
				{
					switch (op1->type)
					{
						case tBOOLEAN:
							op3->data.b=op1->data.b >= op2->data.b;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
 			 				break;

		 				case tINTEGER:
							op3->data.b=op1->data.i >= op2->data.i;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 	 				break;

		 				case tREAL:
							op3->data.b=op1->data.r >= op2->data.r;
							if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;

		 				case tSTRING:

		 					if((strlen(op1->data.s)) >= (strlen (op2->data.s)))

		 					op3->data.b=true;

		 					else op3->data.b=false;

		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
								op3->type=tBOOLEAN;
			 					break;

		 				default:
		 					op3->data.b=false;
		 					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
							op3->type=tBOOLEAN;
		 					break;
					}
				}

				else{
						Error(4);
						break;
 	   				}
				
				
				break;
			}

			case I_ASSIGN:
			{
				

				if((op1->type)==tINTEGER)
				{

					op3->data.i=op1->data.i;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
					op3->type=tINTEGER;

				}
				else if((op1->type)==tREAL)
				{

					op3->data.r=op1->data.r;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
					op3->type=tREAL;
				}
				else if((op1->type)==tBOOLEAN)
				{
					op3->data.b=op1->data.b;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
					op3->type=tBOOLEAN;
				}
				else if(op1->type==tSTRING)
				{
					if (instr->a->type==tVAR)
					{
						op3->data.s=malloc(strlen(op1->data.s)+1);
					    InsertFirst(LGar,op3->data.s);
						strcpy(op3->data.s,op1->data.s);
					}
					else op3->data.s=op1->data.s;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam) || (op3 == tmfunc))
					op3->type=tSTRING;
				}
				
				break;
			}

			case I_PRINT:
			{
				
				if((op1->type)==tINTEGER)
				{
					printf("%d",op1->data.i);
					break;

				}
				else if((op1->type)==tREAL)
				{
					printf("%g",op1->data.r );
					break;
				}
				else if((op1->type)==tBOOLEAN)
				{
					printf("%s",op1->data.b ? "TRUE" : "FALSE" );
					break;
				}
				else if((op1->type)==tSTRING)
				{

				print_my_string(op1->data.s);	//upravi dany string pre spravny vypis
					break;
				}

				break;
			}

			case I_READ:
			{
				
				unsigned int len_max = 128;
				unsigned int current_size = 0;
				char *ptr;
				char *pStr = malloc(len_max);
				InsertFirst(LGar,pStr);
				current_size = len_max;

				if(pStr != NULL)
				{
					int c = EOF;
					unsigned int i = 0;

					while (( c = getc(stdin)) != '\n' && (c != EOF))
					{
						pStr[i++] = (char)c;

						if(i == current_size)
						{
							current_size = i+len_max;
							pStr = realloc(pStr,current_size);		//realokacia premennej ak je vstup rozsiahly
						}
					}
					pStr[i]= '\0';
					//printf ("\n");


					if((op3->type)==tINTEGER)
					{
						long num;
						
						num = strtol(pStr,&ptr,10);
						if (ptr == pStr) 
							 Error(6);
						/*if (*ptr != '\0') 
							Error(6)
    						*/
						op3->data.i=num;
						
					}

					else if (op3->type==tSTRING)
					{
						
						op3->data.s=pStr;
					}

					else if (op3->type==tREAL)

					{
						double num;
						num = strtod(pStr,&ptr);
						op3->data.r=num;
						
						
						
						
							/*    int	what_kind_of_number (char *s)
										{
										    char *endp;
										    double d;
										    long l;
										
										
										    d = strtod(s, &endp);
										    if (s != endp && *endp == `\0')
										        printf("It's a float with value %g\n", d);
										    else
										    {
										        l = strtol(s, &endp, 0);
										        if (s != endp && *endp == `\0')
										            printf("It's an integer with value %ld\n", 1);
										        else
										            return 1;
										    }
										    return 0;
										}*/
					}
					else if (op3->type == tBOOLEAN)

					{
						Error(4);
						break;
					}

					else

						{
								Error(4);
								break;
						}



				}

				
				break;
			}


			case I_LABEL:
			{
				
				break;
			}


			case I_GOTO:	//skokova instrukcia
			{
				
				if(op2 != NULL)
				{
					if(op2->type==tBOOLEAN)
					{
						if(op2->data.b==true)		//podmienka skoku
						{
							break;
						}
					}
					else break;
				}
				lablist->Active=lablist->First;
				while(lablist->Active != NULL)
				{
					/*najde sa dane navestie, na ktore sa ma skocit*/
					if(op1->data.i==lablist->Active->lab_ID)
						instList->active=lablist->Active->ins_ptr;
					lablist->Active=lablist->Active->next;
				}
				break;
			}

			case I_CALL:
			{
				/*ulozi sa zoznam lokalnych premennych*/
				if(LocalFrame != NULL)
					push(&Fstack,&LocalFrame,-1);
				LocalFrame=createFrame(op1->data.s, varList);

				/*ulozi sa navratova adresa instrukcie*/
				push(&Fstack,&instList->active,sizeof(tListItem*));

				/*ulozi sa premenna pre navratovu hodnotu*/
				push(&Fstack,&op3,sizeof(struct FrameVariable*));

				LocalFrame->active=LocalFrame->first;
				while((LocalFrame->active != NULL) && (LocalFrame->active->param==false))	//najde prvy param v zozname
					LocalFrame->active=LocalFrame->active->nextvar;

				while((LocalFrame->active != NULL) && (LocalFrame->active->param==true))
				{
					/*priradenie honoty parametrom funkcie zo zasobnika*/
					switch(LocalFrame->active->type)
					{
						case tINTEGER:
						LocalFrame->active->data.i=*(int *)top(&stack);
						pop(&stack);
						
						break;

						case tREAL:
						LocalFrame->active->data.r=*(double *)top(&stack);
						pop(&stack);
						
						break;

						case tBOOLEAN:
						LocalFrame->active->data.b=*(bool *)top(&stack);
						pop(&stack);
						
						break;

						case tSTRING:
						LocalFrame->active->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
						strcpy(LocalFrame->active->data.s,*(char **)top(&stack));
						InsertFirst(LGar,LocalFrame->active->data.s);
						pop(&stack);
						
						break;

						default:
						break;

					}
					LocalFrame->active=LocalFrame->active->nextvar;
				}

				/*vyhladanie danej funkcie a skok na jej prvu instrukciu*/
				lablist->Active=lablist->First;
				while(lablist->Active != NULL)
				{
					if(lablist->Active->func_name != NULL)
					{
						if(strcmp(op1->data.s, lablist->Active->func_name)==0)
						{
							instList->active=lablist->Active->ins_ptr;
							bcall=true;
						}
					}
					lablist->Active=lablist->Active->next;
				}

				break;
			}

			case I_RETURN:
			{

				op3=*(tFrameVariable**)top(&Fstack);	//nacitanie premennej, do ktorej sa ulozi navratova hodnota
				pop(&Fstack);

				op3->type=LocalFrame->first->type;

				/*skopirovanie hodnoty z id funkcie do navratovej premennej*/
				switch(LocalFrame->first->type)
				{
					case tINTEGER:
						op3->data.i=LocalFrame->first->data.i;

						break;

						case tREAL:

						op3->data.r=LocalFrame->first->data.r;

						break;

						case tBOOLEAN:
						op3->data.b=LocalFrame->first->data.b;
						break;

						case tSTRING:

						op3->data.s=malloc(strlen(LocalFrame->first->data.s));
					    InsertFirst(LGar,op3->data.s);
						strcpy(op3->data.s, LocalFrame->first->data.s);

						break;

						default:
						break;
				}

				instList->active=*(struct listItem**)top(&Fstack);	//nacitanie navratovej adresy instrukcie
				pop(&Fstack);

				frameFree(LocalFrame);		//odstranenie lokalnej tabulky danej funkcie

				if(S_empty(&Fstack)==false)
				{
					LocalFrame=*(tListofVariables **)top(&Fstack);		//zo zasobniku sa nacita aktivna lokalna tabulka
					pop(&Fstack);
				}
				else LocalFrame=NULL;


				break;
			}

			case I_PUSH:
			{
				
				/*ulozenie hodnoty na zasobnik*/	
				switch(op1->type)
				{
					case tINTEGER:
					push(&stack,&op1->data.i,sizeof(int));
					break;

					case tREAL:
					push(&stack,&op1->data.r,sizeof(double));
					break;

					case tBOOLEAN:
					push(&stack,&op1->data.b,sizeof(bool));
					break;

					case tSTRING:
					push(&stack,&op1->data.s,strlen(op1->data.s)+1);
					break;

					default:
					break;

				}

				break;
			}

			case I_LENGTH:
			{
				/*nacitanie argumentu*/
				tmp1->type=tSTRING;
				tmp1->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp1->data.s,*(char **)top(&stack));
				pop(&stack);

			    InsertFirst(LGar,tmp1->data.s);

				op3->data.i=length_func(convert_my_string(tmp1->data.s));	//vstavana funkcia
				op3->type=tINTEGER;

				break;
			}

			case I_SORT:
			{
			/*nacitanie argumentu*/
				tmp1->type=tSTRING;
				int str_len_sort=strlen(*(char **)stack.Top->data);         // spravim si kopiu, potrebujem dlzku
				tmp1->data.s=malloc(str_len_sort+1);                        // naalokujem meisto
				strcpy(tmp1->data.s,*(char **)top(&stack));                 // skopirujem si
				pop(&stack);
				InsertFirst(LGar,tmp1->data.s);                             // ulozim do GC

        			 char * converted_string = convert_my_string(tmp1->data.s);  // spravim si pomocny ukazovatel so skonvertovanym stringom
                		str_len_sort=strlen(converted_string);                      // spocitam si jeho dlzku kvoli parametru funkcie sort
				sort_func(converted_string,1,str_len_sort);		            //vstavana funkcia
				op3->type=tSTRING;



				break;
			}

			case I_FIND:
			{
				/*nacitanie argumentu*/
				tmp2->type=tSTRING;
				tmp2->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp2->data.s,*(char **)top(&stack));
				pop(&stack);
				InsertFirst(LGar,tmp2->data.s);

				tmp1->type=tSTRING;
				tmp1->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp1->data.s,*(char **)top(&stack));
				pop(&stack);
				InsertFirst(LGar,tmp1->data.s);

				op3->data.i=find_func(convert_my_string(tmp1->data.s), convert_my_string(tmp2->data.s));		//vstavana funkcia
				op3->type=tINTEGER;


				break;

			}

			case I_COPY:
			{
				/*nacitanie argumentu*/
				tmparam->type=tINTEGER;
				tmparam->data.i=*(int *)top(&stack);
				pop(&stack);

				tmp2->type=tINTEGER;
				tmp2->data.i=*(int *)top(&stack);
				pop(&stack);

				tmp1->type=tSTRING;
				tmp1->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp1->data.s,*(char **)top(&stack));
				pop(&stack);

				InsertFirst(LGar,tmp1->data.s);

				len=strlen(convert_my_string(tmp1->data.s));
				if(tmparam->data.i > len)
				{
					Error(99);
				}
				op3->data.s=copy_func(convert_my_string(tmp1->data.s), tmp2->data.i, tmparam->data.i);		//vstavana funkcia
				op3->type=tSTRING;

				break;
			}


			default:
			break;

	}	//KONIEC SWITCHU

	if(bcall==false)
	{
		instList->active=instList->active->nextItem;
		if (instList->active == NULL)
			run=false;

	}
	else bcall=false;

	if(bop1 == true )
	{
		free(op1);
		bop1=false;
	}


	if(bop2 == true )
	{
		free(op2);
		bop2=false;
	}




}	//KONIEC WHILE(RUN)

if(GlobalFrame != NULL)
	frameFree(GlobalFrame);		//uvolnenie globalnej tabulky 

/*uvolnenie pomocnych premennych*/
free(tmp1);
free(tmp2);
free(tmparam);
free(tmfunc);


}  //KONIEC FUNKCIE INTERPRETATION LOOP


// Funkcia printchar sluzi na zabranie vytlacenia netlacitelneho znaku, napr. koniec riadku, tabulator

void printchar(unsigned char theChar)
{
	switch (theChar)
	{
		case '\n':
		printf("\\n");
		break;
		case '\r':
		printf("\\r");
		break;
		case '\t':
		printf("\\t");
		break;
		case '\f':
		printf("\\f");
		break;
	}
}

// Funkcia print_my_string sluzi na vytlacenie retazca podla formatovaneho vystupu jazyka IFJ14 (funkcia write)

void print_my_string (char *str)
{
	int i=0;
	int parity=0;
	while ((str[i]!='\0'))                                                  // kym nie si na konci
	{
		if (str[i]=='#' && parity % 2 == 0)                                                    // ak narazis na escape sekvenciu
		{
			int j=0;
			int count=i+1;
			while (str[count]>='0' && str[count]<='9')                      // spracuj ju ako cislo
			{
				count++;
				j++;
			}
			char convert[j+1];
			strncpy(convert,(str+i+1),j);
			convert[j]='\0';
            int value=atoi(convert);                                        // skonvertuj
            printf ("%c",value);                                            // a vytlac
			i=count;
        }
		if (str[i]!='\'')                                                   // ak si nenarazil na apostrof
		{
			if (str[i]<32)
			{
				printchar (str[i]);                                         // znak so spatnym lomitkom
            }
			else
			{
				printf ("%c",str[i]);                                       // vytlac znak
            }
        }
        else
        {
            parity++;                                                       // ak sedi parita a dalsi znak je apostrof, vytlac ho
            if (parity % 2 == 0 && str[i+1]=='\'') printf ("%c",str[i++]);
		}


        i++;

    }
}

// Funkcia convert_my_string sluzi na konverzovanie retazca pre pracu s nim, napr tlac, ale hlavne vstavane funkcie

char * convert_my_string (char *str)
{
	int i=0;
	int k=0;
	char * string = malloc (sizeof(char)*(strlen(str)));
	InsertFirst(LGar,string);
	int parity=0;
	while ((str[i]!='\0'))
	{
		if (str[i]=='#' && parity % 2 == 0)
		{
			int j=0;
			int count=i+1;
			while (str[count]>='0' && str[count]<='9')
			{
				count++;
				j++;
			}
			char convert[j+1];
			strncpy(convert,(str+i+1),j);
			convert[j]='\0';
			int value=atoi(convert);
			string[k]=value;
			k++;
			i=count;
		}
		if (str[i]!='\'')
		{
			if (str[i]<32)
			{
				string[k]=convertchar(str[i]);
			}
			else
			{
				string[k]=str[i];
				k++;
			}
		}
		else
		{
            parity++;
            if (parity % 2 == 0 && str[i+1]=='\'')
            {
                string[k]=str[i++];
                k++;
            }
        }

        i++;

    }
	string[k]='\0';
	return string;
}

// Funkcia convertchar prekonvertuje znak na znak uzivany v jazyku C

char convertchar(unsigned char theChar)
{
	switch (theChar)
	{
		case '\n':
		return '\n';

		case '\r':
		return '\r';

		case '\t':
		return '\t';

		case '\f':
        return '\f';
	}
	return 0;
}
