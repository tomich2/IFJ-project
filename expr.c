#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexical.h"
#include "expr.h"
#include "parser.h"
#include "stack.h"



typedef enum nonterminal { handle /*<*/ , Exp /* EXPR */ , Lexp /* Param Expr */ } nonterm;

action PrecTable [16] [16] =
	{
	 //      +,  -,  *,  /,  =,  <,  >,  <=,  >=,  <>,  i,  f,  ,,  (,  ),  $
	 {/* + */R,  R,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* - */R,  R,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* * */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* / */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* = */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* < */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* > */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/*<= */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/*>= */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/*<> */L,  L,  L,  L,  R,  R,  R,   R,   R,   R,  L,  L,  R,  L,  R,  R},
	 {/* i */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  Q,  Q,  R,  E,  R,  R},
 	 {/* f */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  Q,  Q,  R,  Q,  R,  R},
 	 {/* , */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  R,  L,  R,  Q},
	 {/* ( */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  L,  L,  E,  Q},
	 {/* ) */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  Q,  Q,  R,  Q,  R,  R},
	 {/* $ */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  Q,  L,  Q,  Q}
	};
oprs converttooprs(identita id, bool tab)
{
 switch (id)
	{
	case OpPlus:
	    return EPlus;
	case OpMinus:
	    return EMinus;
	case OpKrat:
	    return EMulti;
	case OpDiv:
	    return EDiv;
	case OpRovny:
	    return EEqual;
	case OpMensi:
	    return ELess;
	case OpVacsi:
	    return EGreater;
	case OpMensiR:
	    return ELorEq;
	case OpVacsiR:
	    return EGorEq;
	case OpNerovny:
	    return EDiff;
	case ID:
	case KwSort:
	case KwFind:
	    return EId;
	case DtInteger:
	     if(!tab) return ETermInt;
	     else return ETerm;
	case DtReal:
	     if(!tab) return ETermReal;
	     else return ETerm;
	case BooleanTrue:
	case BooleanFalse:
	     if(!tab) return ETermBool;
	     else return ETerm;
	case DtString:
	     if(!tab) return ETermString;
	     else return ETerm;
	case OpCiarka:
	    return EComma;
	case OpLZat:
	    return ELpar;
	case OpPZat:
	    return ERpar;
	default: return EEnd;
	}
}

int GetRule(int a, int b)
{
 return PrecTable[a][b];
}
int Reduction(Stack *stack, T_ParserItem *in, Tabs *STab)
{
  ERROR_MSG err;
  int rule;
  nont op1, op2;
  T_ParserItem tmp;
	tmp=*(T_ParserItem*) (top(stack));
	pop(stack);
	if (tmp.type==0) //mame neterminal
		{
		 if(tmp.value.nonterm.type!=Exp) return -1; //ak to nie je E -chyba

		 op1.type=tmp.value.nonterm.type;
		 op1.index=tmp.value.nonterm.index; // semantika - prvy operand E
		 op1.d_type=tmp.value.nonterm.d_type;

		 tmp= *(T_ParserItem *) top(stack); //novy pop
		 pop(stack);
		 if(tmp.type==0) return -1; //ak to je neterminal chyba (ocakavame ciarku alebo operator)
		 if(converttooprs(tmp.value.term.type,false)==EComma) //ak je to ciarka - mame E,
			{
			 tmp= *(T_ParserItem *) top(stack); // novy pop
			 pop(stack);
			 if(tmp.type==0) //ak to je neterminal co ocakavame
				{
				 if(tmp.value.nonterm.type==handle) return -1; //ak je to handle -chyba

				  op2.type=tmp.value.nonterm.type;
				  op2.index=tmp.value.nonterm.index; // semantika - druhy operand E alebo L
				  op2.d_type=tmp.value.nonterm.d_type;

				  if(op2.type==Exp) rule=16; else rule=15; // semantika - priradenie pravidla

				 tmp= *(T_ParserItem *) top(stack); //novy pop
			 	 pop(stack);
				 if(tmp.type==1) return -1; //ak je to terminal -chyba
				 if(tmp.value.nonterm.type==handle)	{
									// ExprSem(rule,&op1,&op2); //semanticke akcie
									 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 		 else if(err==SEMANTIC_ERR) return -2;
									 in->value.nonterm.index=op1.index;
									 in->value.nonterm.d_type=op1.d_type;
									 return 2; // ak sme dostali handle co ocakavame redukujeme na LExp -> Exp, (L)Exp
									}
				 else return -1;			  // ak to nie je handle -chyba
				}
			 else return -1;
			}
		 if(converttooprs(tmp.value.term.type,false)<=9) //ak je to operator - mame E op
			{

			 rule=converttooprs(tmp.value.term.type,false)+1; // semantika - priradenie pravidla

			 tmp=*(T_ParserItem *) top(stack); // novy pop
                         pop(stack);
                         if(tmp.type==0) //ak to je neterminal co ocakavame
                                {
                                 if(tmp.value.nonterm.type!=1) return -1; //ak to nie je E -chyba

				 op2.type=tmp.value.nonterm.type;
				 op2.index=tmp.value.nonterm.index; // semantika - druhy operand E
				 op2.d_type=tmp.value.nonterm.d_type;

                                 tmp= *(T_ParserItem *) top(stack); //novy pop
                                 pop(stack);
                                 if(tmp.type==1) return -1; //ak je to terminal -chyba
                                 if(tmp.value.nonterm.type==handle)
								{
								 //ExprSem(rule,&op1,&op2); //semanticke akcie
								 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 	 else if(err==SEMANTIC_ERR) return -2;
								 in->value.nonterm.index=op1.index;
								 in->value.nonterm.d_type=op1.d_type;
								 return 1; // ak sme dostali handle co ocakavame redukujeme na Exp -> Exp op Exp
								}
                                 else return -1;                          // ak to nie je handle -chyba
                                }
                         else return -1;

			}
		 return -1;
		}
	else
		{
		 if((converttooprs(tmp.value.term.type,false)==10) || (converttooprs(tmp.value.term.type,true)==11))
			{

			 op1.type=converttooprs(tmp.value.term.type,false);
			 op1.index=(char *)malloc(strlen(tmp.value.term.index)+1); //alokacia daneho retazca
	  		 if (op1.index==NULL)
	  		 { //kontrola navratovej hodnoty malloc
				return -1;
			 }
	  		 strcpy(op1.index,tmp.value.term.index); //zkopirovanie retazca do naalokovanej pameti
			 free(tmp.value.term.index);

			 op1.d_type=converttooprs(tmp.value.term.type,true);
			 //free( op1.index);
		 	 tmp=*(T_ParserItem *) top(stack); //novy pop
		 	 pop(stack);
		 	 if(tmp.type==1) return -1; //ak je to terminal -chyba

			 rule=17; //semantika - priradenie pravidla

		 	 if(tmp.value.nonterm.type==handle)
							{
							 //ExprSem(rule,&op1,NULL); //semanticke akcie
							 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 else if(err==SEMANTIC_ERR) return -2;
							 in->value.nonterm.index=op1.index;
							 in->value.nonterm.d_type=op1.d_type;
							 return 1; // ak sme dostali handle co ocakavame redukujeme na Exp -> term
							}
		 	 else return -1;			  // ak to nie je handle -chyba
			}
		 if(converttooprs(tmp.value.term.type,false)==ERpar) // mame pravu zatvorku
			{
		 	 tmp= *(T_ParserItem *) top(stack); //novy pop
		 	 pop(stack);
		 	 if(tmp.type==1) // mame terminal
				{
				 if(converttooprs(tmp.value.term.type,false)!=ELpar) return -1; // ak to nie je lava zatvorka -chyba
				 else
				 	{
					 tmp= *(T_ParserItem *) top(stack); //novy pop
			 	 	 pop(stack);
		 	 		 if(tmp.type==0) return -1; //ak je to neterminal -chyba
				 	 if(converttooprs(tmp.value.term.type,false)!=EId)	return -1; // ak sme nedostali identifikator funkcie -chyba

		 			 op1.type=converttooprs(tmp.value.term.type,false);
					 op1.index=tmp.value.term.index; // semantika - prvy operand E
					 //op1.d_type=tmp.value.term.d_type;

					 tmp= *(T_ParserItem *) top(stack); //novy pop
				 	 pop(stack);
				 	 if(tmp.type==1) return -1; //ak je to terminal -chyba
				 	 if(tmp.value.nonterm.type==handle)
							{
							 rule=12; //semantika - priradenie pravidla
							 //ExprSem(rule,&op1,NULL); //semanticke akcie
							 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 else if(err==SEMANTIC_ERR) return -2;
							 in->value.nonterm.index=op1.index;
							 in->value.nonterm.d_type=op1.d_type;
						 	 return 1; // ak sme dostali handle co ocakavame redukujeme na Exp->f()
							}
				 	 else return -1;			  // ak to nie je handle -chyba
					}
				}
			 else // mame neterminal - E)
				{
				  op1.type=tmp.value.nonterm.type;
		 		  op1.index=tmp.value.nonterm.index; // semantika - prvy operand E alebo L
				  op1.d_type=tmp.value.nonterm.d_type;

				  tmp= *(T_ParserItem *) top(stack); //novy pop
                                  pop(stack);
				  if(converttooprs(tmp.value.term.type,false)!=ELpar) return -1; // ak to nie je lava zatvorka -chyba
                                  else
                                        {
                                         tmp= *(T_ParserItem *) top(stack); //novy pop
                                         pop(stack);
                                         if(tmp.type==0) //ak je to neterminal
						{
						 if (tmp.value.nonterm.type==handle)
							{
							 if(op1.type==Exp) rule=11; else return -1; // semantika - priradenie pravidla
							 //ExprSem(rule,&op1,NULL); //semanticke akcie
							 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 else if(err==SEMANTIC_ERR) return -2;
							 in->value.nonterm.index=op1.index;
							 in->value.nonterm.d_type=op1.d_type;
							 return 1; // a je to handle tak redukujeme E->(E)
							}
					 	 else return -1;
						}
					 else //ak to je terminal
						{

                                         	 if(converttooprs(tmp.value.term.type,false)!=EId)    return -1; // ak sme nedostali identifikator funkcie -chyba
						 op2.type=converttooprs(tmp.value.term.type,false);
						 op2.index=tmp.value.term.index; // semantika - druhy operand Eid
						// op2.d_type=tmp.value.nonterm.d_type;

                                         	 tmp= *(T_ParserItem *) top(stack); //novy pop
                                         	 pop(stack);
                                         	 if(tmp.type==1) return -1; //ak je to terminal -chyba
                                         	 if(tmp.value.nonterm.type==handle)
							{
							 if(op1.type==Exp) rule=13; else rule=14; // semantika - priradenie pravidla
							 //ExprSem(rule,&op1,&op2,glob,loc); //semanticke akcie
							 if((err=ExprSem(rule,&op1,&op2,STab))==EXPRESSION_ERR) return -3;
							 else if(err==SEMANTIC_ERR) return -2;
							 in->value.nonterm.index=op1.index;
							 in->value.nonterm.d_type=op1.d_type;
							 return 1; // ak sme dostali handle co ocakavame redukujeme na Exp->f(E) alebo Exp->f(L)
							}
						 else return -1;  // ak to nie je handle -chyba
						}
                                        }


				}
			}

		}

  return -1;
}

T_ParserItem *GetTerm(Stack *stack, bool handle)
{
  T_ParserItem tmp;
  T_ParserItem *tmp2;
  T_ParserItem hdl;
  hdl.type=0;
  hdl.value.nonterm.type=0;
  tmp= *(T_ParserItem *) top(stack); //novy pop
  if (tmp.type==0)
	{
	  pop(stack);
	  tmp2=(T_ParserItem *) top(stack);
	  if (handle) push(stack,&hdl,-1);
	  push(stack,&tmp,-1);
	  return tmp2;
	}
  else
	{
	  tmp2=(T_ParserItem *) top(stack);
	  if (handle) push(stack,&hdl,-1);
	  return tmp2;
	}
}

ERROR_MSG ExprParse( htab_t *glob, htab_t *loc, T_vartype *dt, tListOfInstr *InstL)
{
  Stack stack;
  init(&stack, sizeof(T_ParserItem));
  int reduct;
  ERROR_MSG err;

  Tabs STab;
  STab.glob=glob;
  STab.loc=loc;
  STab.InstL=InstL;
  STab.is_cmp=false;
  STab.is_comm=false;
  STab.is_func=false;
  STab.was_func=false;
  T_ParserItem in;
  T_ParserItem *top;
  T_ParserItem *tmp_top;
  Variable *a;
  void *tmp=NULL;
  bool is_exp=false;

  in.type=TERMINAL;
  in.value.term.type=EndOfFile;
  push(&stack, &in, -1);

  if((converttooprs(token->identity,true)==EId) || (converttooprs(token->identity,true)==ETerm))
  {
	  in.value.term.index=(char *)malloc(strlen(token->mem)+1); //alokacia daneho retazca
	  if ( in.value.term.index==NULL)
	  { //kontrola navratovej hodnoty malloc

		return -1;
	  }
	  strcpy( in.value.term.index,token->mem); //zkopirovanie retazca do naalokovanej pameti
  }
  else  in.value.term.index=NULL;

  free (token->mem);
  token->mem=NULL;
  top=GetTerm(&stack, 0);
  while ((converttooprs(top->value.term.type,false)!=EEnd) || (converttooprs(token->identity,false)!=EEnd))
	{

	 top=GetTerm(&stack, 0);
	 switch (GetRule(converttooprs(top->value.term.type,true),converttooprs(token->identity,true)))
		{
		 case E:
			 in.type=TERMINAL;
			 in.value.term.type=token->identity;
			 push(&stack, &in , -1);
			 if ((err=get_token())!=0) return err;
	 		 if(token->identity!=DtString) strtoupper(&token->mem);
			 tmp_top=GetTerm(&stack, 0);
		if((converttooprs(token->identity,true)>=4) && (converttooprs(token->identity,true)<=9)) STab.is_cmp=true;
		if((converttooprs(token->identity,true)==ELpar) && (converttooprs(tmp_top->value.term.type,true)==EId)) STab.is_func=true;

			 if((converttooprs(token->identity,true)==EId) || (converttooprs(token->identity,true)==ETerm))
  			 {
				  in.value.term.index=(char *)malloc(strlen(token->mem)+1); //alokacia daneho retazca
				  if ( in.value.term.index==NULL)
				  { //kontrola navratovej hodnoty malloc

					return -1;
				  }
			 	  strcpy( in.value.term.index,token->mem); //zkopirovanie retazca do naalokovanej pameti
  			 }
			 else  in.value.term.index=NULL;

			 free (token->mem);
			 token->mem=NULL;
			 top=GetTerm(&stack, 0);
			 break;
		 case L:
			 top=GetTerm(&stack, 1);
			 in.type=TERMINAL;
			 in.value.term.type=token->identity;
			 push(&stack, &in , -1);
			if((converttooprs(token->identity,true)>=4) && (converttooprs(token->identity,true)<=9)) STab.is_cmp=true;
			if((converttooprs(token->identity,true)==EComma) && (converttooprs(top->value.term.type,true)==ELpar)) STab.is_comm=true;
			 if ((err=get_token())!=0) return err;
			 if(token->identity!=DtString) strtoupper(&token->mem);
			 tmp_top=GetTerm(&stack, 0);


			if((converttooprs(token->identity,true)==ELpar) && (converttooprs(tmp_top->value.term.type,true)==EId)) STab.is_func=true;

			 if((converttooprs(token->identity,true)==EId) || (converttooprs(token->identity,true)==ETerm))
  			 {
				  in.value.term.index=(char *)malloc(strlen(token->mem)+1); //alokacia daneho retazca
				  if ( in.value.term.index==NULL)
				  { //kontrola navratovej hodnoty malloc

					return -1;
				  }
			 	  strcpy( in.value.term.index,token->mem); //zkopirovanie retazca do naalokovanej pameti
  			 }
			 else  in.value.term.index=NULL;

			 free (token->mem);
			 token->mem=NULL;

			 top=GetTerm(&stack, 0);
			 break;
		 case R:
			is_exp=true;
			 if ((reduct=Reduction(&stack,&in,&STab))>0)
				{

				  in.type=NONTERMINAL;
			 	  in.value.nonterm.type=reduct;
				  push(&stack, &in , -1);
				  top=GetTerm(&stack, 0);
				}
			 else
				{

				  S_erase(&stack);
  			 	  if(reduct==-1) return SYNTAX_ERR;
				  if(reduct==-2) return SEMANTIC_ERR;
				  if(reduct==-3) return EXPRESSION_ERR;
				}
			 break;
		 case Q:

			 S_erase(&stack);
  			 return SYNTAX_ERR;
		}

	}
S_erase(&stack);
if(is_exp)
{
	if((tmp=htab_search(STab.loc,(char *) in.value.nonterm.index))==NULL) tmp=htab_search(STab.glob,(char *) in.value.nonterm.index);
	if(tmp==NULL)
	{
		MakeVariable(&a,in.value.nonterm.d_type,in.value.nonterm.index);
		generator(STab.InstL,I_ASSIGN,a,NULL,TMPU);
	}
	if((strcmp(in.value.nonterm.index,TMPU)!=0) && (strcmp(in.value.nonterm.index,TMPU2)!=0) && (strcmp(in.value.nonterm.index,TMFunc)!=0) && (strcmp(in.value.nonterm.index,TMParam)!=0)) free(in.value.nonterm.index);
	//if(in.value.nonterm.index!=NULL) free(in.value.nonterm.index);
	*dt=in.value.nonterm.d_type;
	S_erase(&stack);
	return EVERYTHINGSOKAY;
}
else
{
	S_erase(&stack);
	return SYNTAX_ERR;
}
}


ERROR_MSG MakeVariable(Variable **a, T_vartype type, void *data)
{

	*a=(Variable *) malloc(sizeof(Variable));
	if(a==NULL) return INTERN_INTERPRETATION_ERR;
	(*a)->type=type;

	switch((*a)->type)
	{
		case tBOOLEAN:  (*a)->data.i=atoi((char *) data);
				break;
		case tINTEGER:  (*a)->data.i=atoi((char *) data);
				break;
		case tREAL: (*a)->data.r=atof((char *) data);
				break;
		case tSTRING:
		case tVAR:
				;
				char *str=(char *) data;
				(*a)->data.s=malloc(strlen(str)+1);
				if((*a)->data.s==NULL)
				{
					free(a);
					return INTERN_INTERPRETATION_ERR;
				}
				strcpy((*a)->data.s,str);
				break;
		default:
			fprintf(stderr,"neznama chyba\n");
			return SEMANTIC_ERR;
	}
	return EVERYTHINGSOKAY;
}

ERROR_MSG ExprSem(int rule, nont *op1, nont *op2, Tabs *STab)
{
	//rule=rule+1;
	void *tmp=NULL;
	bool vst=true;
	T_vartype tp;
	int inst=-1;
	char *res;
	Variable *a,*b;
	a=b=NULL;
	if(STab->is_cmp) res=TMPU2;
	else res=TMPU;
	
	if(STab->is_func && !STab->is_comm)/* printf("prvy parameter %s \n", op1->index);*/res=TMParam;
	if(STab->is_func && STab->is_comm)
	{
		STab->is_func=false;
		STab->is_comm=false;
		if(STab->is_cmp) res=TMPU;
		else res=TMPU2;
	}

	switch(rule)
	{
		case 1: //E->E+E
			if(op1->d_type==tINTEGER)
			{
				if(op2->d_type==tINTEGER) tp=tINTEGER;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri scitani\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else if(op1->d_type==tREAL)
			{
				if(op2->d_type==tINTEGER) tp=tREAL;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri scitani\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else if((op1->d_type==tSTRING) && (op2->d_type==tSTRING)) tp=tSTRING;
			else
			{
				fprintf(stderr,"nespravne typy operandov pri scitani\n");
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return EXPRESSION_ERR;
			}

			inst=I_ADD;


			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			if((tmp=htab_search(STab->loc,(char *) op2->index))==NULL) tmp=htab_search(STab->glob,(char *) op2->index);
			if(tmp==NULL) MakeVariable(&b,op2->d_type,op2->index);
			else MakeVariable(&b,tVAR,op2->index); //doriesit chybove stavy

			generator(STab->InstL,inst,a,b,res);


			op1->d_type=tp;
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
		case 2: //E->E-E
			inst=I_SUB;
		case 3: //E->E*E
			;
			if(inst<0) inst=I_MUL;
			if(op1->d_type==tINTEGER)
			{
				if(op2->d_type==tINTEGER) tp=tINTEGER;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri odcitani alebo nasobeni\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else if(op1->d_type==tREAL)
			{
				if(op2->d_type==tINTEGER) tp=tREAL;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri odcitani alebo nasobeni\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else
			{
				fprintf(stderr,"nespravne typy operandov pri odcitani alebo nasobeni\n");
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return EXPRESSION_ERR;
			}




			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			if((tmp=htab_search(STab->loc,(char *) op2->index))==NULL) tmp=htab_search(STab->glob,(char *) op2->index);
			if(tmp==NULL) MakeVariable(&b,op2->d_type,op2->index);
			else MakeVariable(&b,tVAR,op2->index); //doriesit chybove stavy

			generator(STab->InstL,inst,a,b,res);

			op1->d_type=tp;
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
		case 4: //E->E/E
			;
			if(inst<0) inst=I_DIV;
			if(op1->d_type==tINTEGER)
			{
				if(op2->d_type==tINTEGER) tp=tREAL;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri deleni\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else if(op1->d_type==tREAL)
			{
				if(op2->d_type==tINTEGER) tp=tREAL;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri deleni\n");
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else
			{
				fprintf(stderr,"nespravne typy operandov pri deleni3\n");
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return EXPRESSION_ERR;
			}



			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			if((tmp=htab_search(STab->loc,(char *) op2->index))==NULL) tmp=htab_search(STab->glob,(char *) op2->index);
			if(tmp==NULL) MakeVariable(&b,op2->d_type,op2->index);
			else MakeVariable(&b,tVAR,op2->index); //doriesit chybove stavy

			generator(STab->InstL,inst,b,a,res);

			op1->d_type=tp;
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
		case 5://E->E=E
			;
			if(inst<0) inst=I_EQUAL_CMP;
		case 6://E->E<E
			;
			if(inst<0) inst=I_LESS_CMP;
		case 7://E->E>E
			if(inst<0) inst=I_GR_CMP;
		case 8://E->E<=E
			if(inst<0) inst=I_LESS_EQUAL_CMP;
		case 9://E->E>=E
			if(inst<0) inst=I_GR_EQUAL_CMP;
		case 10://E->E<>E
			if(inst<0) inst=I_NOT_EQUAL_CMP;
			if(op1->d_type == op2->d_type) tp=tBOOLEAN;
			else
			{
				fprintf(stderr,"nespravne typy operandov pri operaciach porovnania\n");
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return EXPRESSION_ERR;
			}


			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			if((tmp=htab_search(STab->loc,(char *) op2->index))==NULL) tmp=htab_search(STab->glob,(char *) op2->index);
			if(tmp==NULL) MakeVariable(&b,op2->d_type,op2->index);
			else MakeVariable(&b,tVAR,op2->index); //doriesit chybove stavy

			generator(STab->InstL,inst,b,a,TMPU);

			op1->d_type=tp;
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
		case 11: //E->(E)
			return EVERYTHINGSOKAY;
		case 12://E->f()
			
		  	if(STab->is_cmp) res=TMPU2;
    		  	else res=TMPU;
			STab->is_func=false;
			if(STab->was_func) res=TMFunc;
			tmp=htab_search(STab->glob,op1->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op1->index);
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);

				return SEMANTIC_ERR;
			}
			else
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op1->index);
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);

					return SEMANTIC_ERR;
				}
      			if(strlen(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types)>1)
			{
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);

				fprintf(stderr,"typy parametrov funkcie sa nezhodujuuuuu\n");
				return EXPRESSION_ERR;
			}
			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);


			MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			generator(STab->InstL,I_CALL,a,NULL,res);
			STab->was_func=true;

			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			op1->index=res;
			return EVERYTHINGSOKAY;

		case 13://E->f(E)
			if(STab->is_cmp) res=TMPU2;
    		  	else res=TMPU;
			STab->is_func=false;
			if(STab->was_func) res=TMFunc;
			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			generator(STab->InstL,I_PUSH,a,NULL,NULL);

			if(strcmp(op2->index,"LENGTH")==0)
			{
				if((int) op1->d_type!=tSTRING) vst=false;
				op1->d_type=tINTEGER;
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				op1->index=res;

				if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
				if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
				else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

				generator(STab->InstL,I_PUSH,a,NULL,NULL);

				generator(STab->InstL,I_LENGTH,NULL,NULL,res);
				return vst ? EVERYTHINGSOKAY:EXPRESSION_ERR;
			}

			if(strcmp(op2->index,"SORT")==0)
			{
				if((int) op1->d_type!=tSTRING) vst=false;
				op1->d_type=tSTRING;
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				op1->index=res;

				if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
				if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
				else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

				generator(STab->InstL,I_PUSH,a,NULL,NULL);

				generator(STab->InstL,I_SORT,NULL,NULL,res);
				return vst ? EVERYTHINGSOKAY:EXPRESSION_ERR;
			}

			tmp=htab_search(STab->glob,op2->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op2->index);
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return SEMANTIC_ERR;
			}
			else
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op2->index);
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return SEMANTIC_ERR;
				}
			op2->d_type=get_type( ((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if((int) op1->d_type!=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,1))
			{
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				fprintf(stderr,"typy parametrov funkcie sa nezhoduju\n");
				return EXPRESSION_ERR;
			}

			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);


			MakeVariable(&a,tVAR,op2->index); //doriesit chybove stavy
			

			generator(STab->InstL,I_CALL,a,NULL,res);

			STab->was_func=true;
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0) && (strcmp(op1->index,TMParam)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2 )!=0) ) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
			break;


		case 14://E->f(L)
			
			if(STab->was_func) res=TMFunc;

			if(strcmp(op2->index,"FIND")==0)
			{
				if(strcmp(op1->index,"ss")!=0) vst=false;
				op1->d_type=tINTEGER;
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				op1->index=res;
				generator(STab->InstL,I_FIND,NULL,NULL,res);
				return vst ? EVERYTHINGSOKAY:EXPRESSION_ERR;
			}

			if(strcmp(op2->index,"COPY")==0)
			{
				if(strcmp(op1->index,"sii")!=0) vst=false;
				op1->d_type=tSTRING;
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				op1->index=res;
				generator(STab->InstL,I_COPY,NULL,NULL,res);
				return vst ? EVERYTHINGSOKAY:EXPRESSION_ERR;
			}
			tmp=htab_search(STab->glob,op2->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op2->index);
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				return SEMANTIC_ERR;
			}
			else
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op2->index);
					if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
					if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
					return SEMANTIC_ERR;
				}
			op2->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);

			int length = strlen(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types);
  			char *str;
			char *str2=((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types;
  			str = (char *) malloc(length);
  			strcpy(str, &str2[1]);

			if(strcmp(str,op1->index)!=0)
			{
				free(str);
				if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
				if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
				fprintf(stderr,"typy parametrov funkcie sa nezhoduju\n");
				return EXPRESSION_ERR;
			}
			free(str);

			MakeVariable(&a,tVAR,op2->index); //doriesit chybove stavy


			generator(STab->InstL,I_CALL,a,NULL,res);
			STab->was_func=true;
			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=res;
			return EVERYTHINGSOKAY;
			break;
		case 15://L->L,E

			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy
			generator(STab->InstL,I_PUSH,a,NULL,NULL);

			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0) && (strcmp(op1->index,TMParam)!=0)) free(op1->index);

			op2->index=realloc(op2->index,(strlen(op2->index)+2));

			switch(op1->d_type)
			{
				case tBOOLEAN: strcat(op2->index, "b");
						break;
				case tINTEGER: strcat(op2->index, "i");
						break;
				case tREAL: strcat(op2->index, "r");
						break;
				case tSTRING: strcat(op2->index, "s");
						break;
				default:
					fprintf(stderr,"neznama chyba\n");
					return SEMANTIC_ERR;
			}



			op1->index=op2->index;
			return EVERYTHINGSOKAY;
		case 16://L->E,E
			if((tmp=htab_search(STab->loc,(char *) op2->index))==NULL) tmp=htab_search(STab->glob,(char *) op2->index);
			if(tmp==NULL) MakeVariable(&a,op2->d_type,op2->index);
			else MakeVariable(&a,tVAR,op2->index); //doriesit chybove stavy
			generator(STab->InstL,I_PUSH,a,NULL,NULL);
			if((tmp=htab_search(STab->loc,(char *) op1->index))==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
			if(tmp==NULL) MakeVariable(&a,op1->d_type,op1->index);
			else MakeVariable(&a,tVAR,op1->index); //doriesit chybove stavy

			generator(STab->InstL,I_PUSH,a,NULL,NULL);

	
			if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0) && (strcmp(op1->index,TMParam)!=0)) free(op1->index);
			if((strcmp(op2->index,TMPU)!=0) && (strcmp(op2->index,TMPU2)!=0) && (strcmp(op2->index,TMFunc)!=0)) free(op2->index);
			op1->index=(char *)malloc((sizeof(char)*2+1));
			switch(op2->d_type)
			{
				case tBOOLEAN: strcpy(op1->index,"b");
						break;
				case tINTEGER: strcpy(op1->index,"i");
						break;
				case tREAL: strcpy(op1->index,"r");
						break;
				case tSTRING: strcpy(op1->index,"s");
						break;
				default:
					fprintf(stderr,"neznama chyba\n");
					return SEMANTIC_ERR;
			}
			switch(op1->d_type)
			{
				case tBOOLEAN: strcat(op1->index, "b");
						break;
				case tINTEGER: strcat(op1->index, "i");
						break;
				case tREAL: strcat(op1->index, "r");
						break;
				case tSTRING: strcat(op1->index, "s");
						break;
				default:
					fprintf(stderr,"neznama chyba\n");
					return SEMANTIC_ERR;
			}
			//printf("%s\n",(char *)op1->index);

		

			return EVERYTHINGSOKAY;

		case 17://E->term
			if(op1!=NULL)
			{
				if(op1->type==EId)
				{
					tmp=htab_search(STab->loc,(char *) op1->index);
					if(tmp==NULL) tmp=htab_search(STab->glob,(char *) op1->index);
					if(tmp==NULL)
					{
						fprintf(stderr,"premenna %s nebola deklarovana\n",(char *) op1->index);
						if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
						return SEMANTIC_ERR;
					}
					else
						if(((T_VarData *) ((Hitem *) tmp)->data)->is_def==false)
						{
							fprintf(stderr,"premenna %s nebola definovana\n",(char *) op1->index);
							if((strcmp(op1->index,TMPU)!=0) && (strcmp(op1->index,TMPU2)!=0) && (strcmp(op1->index,TMFunc)!=0)) free(op1->index);
							return SEMANTIC_ERR;
						}

				op1->d_type=((T_VarData *) ((Hitem *) tmp)->data)->type;
				return EVERYTHINGSOKAY;
				}

				op1->d_type=(op1->type)-16;
				return EVERYTHINGSOKAY;
			}



	}


  return EVERYTHINGSOKAY;
}
