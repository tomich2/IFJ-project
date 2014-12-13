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



void interpretLoop(tListOfInstr *instList,t_varfunc_list *varList,t_lablist *lablist)
{
	bool run =true;
	bool div0=false;
	int len;

	struct FrameVariable *op1,*op2,*op3;
	tInstruction *instr;
	tListofVariables *LocalFrame=NULL;
	tListofVariables *GlobalFrame=NULL;
	bool bop1, bop2;
	bop1=false;
	bop2=false;
	bool bcall=false;
	Stack Fstack;
	init(&Fstack,sizeof(struct tListofVariables*));
	Stack stack;
	init(&stack,sizeof(struct tListofVariables*));
	



	/**************pomocne**************/

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


	/**************pomocne**************/




	instList->active=instList->first;
	instr=instList->active->Instr;


	GlobalFrame=createGlobFrame(varList);


	while(run)
	{
	instr=instList->active->Instr;

	if(instr->a != NULL)
	{
		if(instr->a->type==tVAR)
		{
			if(strcmp(instr->a->data.s, tmp1->name)==0) op1=tmp1;

			else if(strcmp(instr->a->data.s, tmp2->name)==0) op1=tmp2;

				 else if(strcmp(instr->a->data.s, tmparam->name)==0) op1=tmparam;

					  else if(strcmp(instr->a->data.s, tmfunc->name)==0) op1=tmfunc;

					       else op1=findFrameVar(instr->a, GlobalFrame, LocalFrame);
		}
		else
			{
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

	if(instr->b != NULL)
	{
		if(instr->b->type==tVAR)
		{
			if(strcmp(instr->b->data.s, tmp1->name)==0) op2=tmp1;

			else if(strcmp(instr->b->data.s, tmp2->name)==0) op2=tmp2;

				 else if(strcmp(instr->b->data.s, tmparam->name)==0) op2=tmparam;

					  else if(strcmp(instr->b->data.s, tmfunc->name)==0) op2=tmfunc;

						   else op2=findFrameVar(instr->b, GlobalFrame, LocalFrame);
		}
		else
		{
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

	if(instr->res != NULL)
	{
		if(strcmp(instr->res, tmp1->name)==0) op3=tmp1;

		else if(strcmp(instr->res, tmp2->name)==0) op3=tmp2;

			 else if(strcmp(instr->res, tmparam->name)==0) op3=tmparam;

				  else if(strcmp(instr->res, tmfunc->name)==0) op3=tmfunc;

					   else op3=findFrameDest(instr->res, GlobalFrame, LocalFrame);

	}
	else op3=NULL;


	/*************************************************SWITCH*****************************************************************/

		switch(instList->active->Instr->Iname)
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
					if (op2->data.i == 0)
					{
						div0=true;
						run=false;
						break;
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
					if (op2->data.r == 0.0)
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

					//printf("%s\n",op1->data.s );
				//	op1->data.s=NULL;
				print_my_string(op1->data.s);
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
							pStr = realloc(pStr,current_size);
						}
					}
					pStr[i]= '\n';


					if((op3->type)==tINTEGER)
					{
						long num;
						num = strtol(pStr,&ptr,10);
						op3->data.i=num;
					}

					else if (op3->type==tSTRING)
					{

						op3->data.s=pStr;
					}

					else if (op3->type==tREAL)

					{
						double num;
						num = strtod(pStr,NULL);
						op3->data.r=num;
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


			case I_GOTO:
			{
				
				if(op2 != NULL)
				{
					if(op2->type==tBOOLEAN)
					{
						if(op2->data.b==true)
						{
							break;
						}
					}
					else break;
				}
				lablist->Active=lablist->First;
				while(lablist->Active != NULL)
				{
					if(op1->data.i==lablist->Active->lab_ID)
						instList->active=lablist->Active->ins_ptr;
					lablist->Active=lablist->Active->next;
				}
				break;
			}

			case I_CALL:
			{

				if(LocalFrame != NULL)
					push(&Fstack,&LocalFrame,-1);
				LocalFrame=createFrame(op1->data.s, varList);

				push(&Fstack,&instList->active,sizeof(tListItem*));

				push(&Fstack,&op3,sizeof(struct FrameVariable*));

				LocalFrame->active=LocalFrame->first;
				while((LocalFrame->active != NULL) && (LocalFrame->active->param==false))	//najde prvy param v zozname
					LocalFrame->active=LocalFrame->active->nextvar;

				while((LocalFrame->active != NULL) && (LocalFrame->active->param==true))
				{
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

				op3=*(tFrameVariable**)top(&Fstack);
				pop(&Fstack);

				op3->type=LocalFrame->first->type;

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

				instList->active=*(struct listItem**)top(&Fstack);
				pop(&Fstack);

				frameFree(LocalFrame);

				if(S_empty(&Fstack)==false)
				{
					LocalFrame=*(tListofVariables **)top(&Fstack);
					pop(&Fstack);
				}
				else LocalFrame=NULL;


				break;
			}

			case I_PUSH:
			{

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

				tmp1->type=tSTRING;
				tmp1->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp1->data.s,*(char **)top(&stack));
				pop(&stack);

			    InsertFirst(LGar,tmp1->data.s);

				op3->data.i=length_func(convert_my_string(tmp1->data.s));
				op3->type=tINTEGER;



				break;
			}

			case I_SORT:
			{

				tmp1->type=tSTRING;
				tmp1->data.s=malloc(strlen(*(char **)stack.Top->data)+1);
				strcpy(tmp1->data.s,*(char **)top(&stack));
				pop(&stack);
				InsertFirst(LGar,tmp1->data.s);



				op3->data.s=sort_func(convert_my_string(tmp1->data.s));
				op3->type=tSTRING;


				break;
			}

			case I_FIND:
			{

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

				op3->data.i=find_func(convert_my_string(tmp1->data.s), convert_my_string(tmp2->data.s));
				op3->type=tINTEGER;


				break;

			}

			case I_COPY:
			{
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
				op3->data.s=copy_func(convert_my_string(tmp1->data.s), tmp2->data.i, tmparam->data.i);
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
	frameFree(GlobalFrame);


free(tmp1);
free(tmp2);
free(tmparam);
free(tmfunc);

if(div0==true)
	Error(8);

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
		if (str[i]=='#')                                                    // ak narazis na escape sekvenciu
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
		if (str[i]=='#')
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
