#include "interpret.h"
#include "struct.h"
#include "error.h"
#include "frame.h"
#include "parser.h"


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>

#define mallConst 10


void interpretLoop(tListOfInstr *instList,t_varfunc_list *varList)
{
	bool run =true;
	struct FrameVariable *op1,*op2,*op3;
	tInstruction *instr;
	tListofVariables *LocalFrame=NULL;
	tListofVariables *GlobalFrame=NULL;
	bool bop1, bop2;
	bop1=false;
	bop2=false;
	
	/**************pomocne**************/
	
	struct FrameVariable *tmp1;
	struct FrameVariable *tmp2;
	struct FrameVariable *tmparam; 
	tmp1=malloc(sizeof(struct FrameVariable));
	tmp2=malloc(sizeof(struct FrameVariable)); 
	tmparam=malloc(sizeof(struct FrameVariable));
	
	tmp1->name=TMPU;
	tmp2->name=TMPU2;
	tmparam->name=TMParam;
	
	
	/**************pomocne**************/
	



	instList->active=instList->first;
	instr=instList->active->Instr;


	GlobalFrame=createGlobFrame(varList);


	while(run)
	{



	if(instr->a != NULL)
	{
		if(instr->a->type==tVAR)
		{
			if(strcmp(instr->a->data.s, tmp1->name)==0) op1=tmp1;
				
			else if(strcmp(instr->a->data.s, tmp2->name)==0) op1=tmp2;
					
				 else if(strcmp(instr->a->data.s, tmparam->name)==0) op1=tmparam;
						
					 
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
						strcpy(op1->data.s,instr->a->data.s);
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
					strcpy(op2->data.s,instr->b->data.s);
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
						
				
					   else op3=findFrameDest(instr->res, GlobalFrame, LocalFrame);
	}				   
	else op3=NULL;

		switch(instList->active->Instr->Iname)
		{

			case I_ADD:
			{
	
				if((op1->type)==tINTEGER)
				{
					if((op2->type)==tINTEGER)
					{
		 				op3->data.i=op1->data.i + op2->data.i;		 					 			
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i + op2->data.r;							 				
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
					}

					else if ((op2->type)==tREAL)
					{	
						op3->data.r=op1->data.r + op2->data.r;		 				
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
					}
					
					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i - op2->data.r;
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
					}

					else if ((op2->type)==tREAL)
					{	
						op3->data.r=op1->data.r - op2->data.r;		 				
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
					}

					else if ((op2->type)==tREAL)
					{
						op3->data.r=op1->data.i * op2->data.r;	
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
					}

					else if ((op2->type)==tREAL)
					{	
						op3->data.r=op1->data.r * op2->data.r;		
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
						Error(8);
						break;
					}
							
					if((op1->type)==tINTEGER)
					{	
						op3->data.r=(double)op1->data.i / (double)op2->data.i;		 				
					}
				
					else if ((op1->type)==tREAL)
					{							
						op3->data.r=op1->data.r / (double)op2->data.i;	
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
					}
				
					else if ((op1->type)==tREAL)
					{	
						op3->data.r=op1->data.r / op2->data.r;	
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
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i == op2->data.i;		
			 				break;

		 				case tREAL:
							op3->data.b=op1->data.r == op2->data.r;
			 				break;

		 				// case tSTRING

		 				default:
			 				op3->data.b=false;
			 				break;
					}
					
				}

				else{
						op3->data.b=false; 				
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
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i != op2->data.i;
		 					break;

		 				case tREAL:
							op3->data.b=op1->data.r != op2->data.r;		 				
		 					break;

		 				// case tSTRING

		 				default:
		 					op3->data.b=false;
		 					break;
					}					
				}

				else{
						op3->data.b=true;
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
			 				break;

		 				case tINTEGER:
							op3->data.b=op1->data.i < op2->data.i;
		 	 				break;

		 				case tREAL:
							op3->data.b=op1->data.r < op2->data.r;
			 				break;

		 				// case tSTRING

		 				default:
		 					op3->data.b=false;
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
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i > op2->data.i;
			 				break;

		 				case tREAL:
							op3->data.b=op1->data.r > op2->data.r;
			 				break;

		 				// case tSTRING

		 				default:
		 					op3->data.b=false;
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
		 					break;

		 				case tINTEGER:
							op3->data.b=op1->data.i <= op2->data.i;
		 					break;

		 				case tREAL:
							op3->data.b=op1->data.r <= op2->data.r;	 				
			 				break;

		 				// case tSTRING

		 				default:
		 					op3->data.b=false;
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
 			 				break;

		 				case tINTEGER:
							op3->data.b=op1->data.i >= op2->data.i;
		 	 				break;

		 				case tREAL:
							op3->data.b=op1->data.r >= op2->data.r;	 				
		 					break;

		 				// case tSTRING

		 				default:
		 					op3->data.b=false;
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
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam))
					op3->type=tINTEGER;

				}
				else if((op1->type)==tREAL)
				{	
					op3->data.r=op1->data.r;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam))
					op3->type=tREAL;
				}
				else if((op1->type)==tBOOLEAN)
				{	
					op3->data.b=op1->data.b;
					if((op3 == tmp1) || (op3 == tmp2) || (op3 == tmparam)) 
					op3->type=tBOOLEAN;
				}
				//string
				break;	
			}
			
			case I_PRINT:
			{
				if((op1->type)==tINTEGER)
				{	
					printf("%d",op1->data.i );
					break;

				}
				else if((op1->type)==tREAL)
				{	
					printf("%f",op1->data.r );
					break;
				}
				else if((op1->type)==tBOOLEAN)
				{	
					printf("%s",op1->data.b ? "TRUE" : "FALSE" );
					break;
				}
				else if((op1->type)==tSTRING)
				{	
					printf("%s",op1->data.s);
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
				current_size = len_max;

				if(pStr != NULL)
				{
					int c = EOF;
					unsigned int i = 0;

					while (( c = getc(stdin)) != '\n' && c != EOF)
					{
						pStr[i++] = (char)c;

						if(i == current_size)
						{
							current_size = i+len_max;
							pStr = realloc(pStr,current_size);
						}
					}	
		
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
						num = strtof(pStr,NULL);
						op3->data.r=num;	
					} 

					free(pStr);
					pStr = NULL;
					
				}	


				break;
			}

			default:
			break;
			
	}	//KONIEC SWITCHU


	instList->active=instList->active->nextItem;
	if (instList->active == NULL)
		run=false;
	else instr=instList->active->Instr;

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

frameFree(GlobalFrame);
free(tmp1);
free(tmp2);
free(tmparam);

}  //KONIEC FUNKCIE INTERPRETATION LOOP
