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
					 op1.index=tmp.value.nonterm.index; // semantika - prvy operand E
					 op1.d_type=tmp.value.nonterm.d_type;
		 	 		 
					 tmp= *(T_ParserItem *) top(stack); //novy pop
				 	 pop(stack);
				 	 if(tmp.type==1) return -1; //ak je to terminal -chyba
				 	 if(tmp.value.nonterm.type==handle)
							{
							 rule=17; //semantika - priradenie pravidla
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

ERROR_MSG ExprParse( htab_t *glob, htab_t *loc, T_vartype *dt)
{
  Stack stack;
  init(&stack, sizeof(T_ParserItem));
  int reduct;
  ERROR_MSG err;

  Tabs STab;
  STab.glob=glob;
  STab.loc=loc;
  STab.is_func=false;

  T_ParserItem in;
  T_ParserItem *top;
  T_ParserItem *tmp_top;

  in.type=TERMINAL;
  in.value.term.type=EEnd;
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
		 case L: 
			 top=GetTerm(&stack, 1);
			 in.type=TERMINAL;
			 in.value.term.type=token->identity;
			 push(&stack, &in , -1);
			
			 if ((err=get_token())!=0) return err;
			 
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
if(in.value.nonterm.index!=NULL) free(in.value.nonterm.index);
*dt=in.value.nonterm.d_type;
S_erase(&stack);
return EVERYTHINGSOKAY;
}

ERROR_MSG ExprSem(int rule, nont *op1, nont *op2, Tabs *STab)
{	
	//rule=rule+1;
	void *tmp=NULL;
	T_vartype tp;
	//if(STab->is_func) printf("som vo funkcii\n");	
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
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
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
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else if((op1->d_type==tSTRING) && (op2->d_type==tSTRING)) tp=tSTRING;
			else
			{
				fprintf(stderr,"nespravne typy operandov pri scitani\n");
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return EXPRESSION_ERR;
			}
			
/*
			Variable a,b;
			
			generator(STab->InstL,I_ADD,
			// generate instruction - alloc new*/
			op1->d_type=tp;
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;
		case 2: //E->E-E
		case 3: //E->E*E
			if(op1->d_type==tINTEGER) 
			{
				if(op2->d_type==tINTEGER) tp=tINTEGER;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri odcitani alebo nasobeni\n");
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
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
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else
			{
				fprintf(stderr,"nespravne typy operandov pri odcitani alebo nasobeni\n");
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return EXPRESSION_ERR;
			}
			op1->d_type=tp;
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;
		case 4: //E->E/E
			if(op1->d_type==tINTEGER) 
			{
				if(op2->d_type==tINTEGER) tp=tREAL;
				else if(op2->d_type==tREAL) tp=tREAL;
				else
				{
					fprintf(stderr,"nespravne typy operandov pri deleni\n");
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
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
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
					return EXPRESSION_ERR;
				}
			}
			else
			{
				fprintf(stderr,"nespravne typy operandov pri deleni3\n");
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return EXPRESSION_ERR;
			}
			op1->d_type=tp;
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;
		case 5://E->E=E
		case 6://E->E<E
		case 7://E->E>E
		case 8://E->E<=E
		case 9://E->E>=E
		case 10://E->E<>E
			if(op1->d_type == op2->d_type) tp=tBOOLEAN;
			else 
			{
				fprintf(stderr,"nespravne typy operandov pri operaciach porovnania\n");
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return EXPRESSION_ERR;
			}
			op1->d_type=tp;
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;
		case 11: //E->(E)
			return EVERYTHINGSOKAY;
		case 12://E->f()
			tmp=htab_search(STab->glob,op1->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op1->index);
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return SEMANTIC_ERR;
			}
			else 
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op1->index);
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
					return SEMANTIC_ERR;
				}
			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if(op1->index!=NULL) free(op1->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;	
		
		case 13://E->f(E)
			
			tmp=htab_search(STab->glob,op2->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op2->index);
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return SEMANTIC_ERR;
			}
			else 
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op2->index);
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
					return SEMANTIC_ERR;
				}
			op2->d_type=get_type( ((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if((int) op1->d_type!=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,1))
			{
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				fprintf(stderr,"typy parametrov funkcie sa nezhoduju\n");
				return SEMANTIC_ERR;
			}
			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;	
			break;
		

		case 14://E->f(L)
			tmp=htab_search(STab->glob,op2->index);
			if(tmp==NULL)
			{
				fprintf(stderr,"funkcia %s nebola deklarovana\n",(char *) op2->index);
				if(op1->index!=NULL) free(op1->index);
				if(op2->index!=NULL) free(op2->index);
				return SEMANTIC_ERR;
			}
			else 
				if(((T_FuncData *) ((Hitem *) tmp)->data)->is_def==false)
				{
					fprintf(stderr,"funkcia %s nebola definovana\n",(char *) op2->index);
					if(op1->index!=NULL) free(op1->index);
					if(op2->index!=NULL) free(op2->index);
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
				fprintf(stderr,"typy parametrov funkcie sa nezhoduju\n");
				return SEMANTIC_ERR;
			}
			free(str);

			op1->d_type=get_type(((T_FuncData *) ((Hitem *) tmp)->data)->ret_par_types,0);
			if(op1->index!=NULL) free(op1->index);
			if(op2->index!=NULL) free(op2->index);
			op1->index=NULL;
			return EVERYTHINGSOKAY;
			break;
		case 15://L->L,E
			free(op1->index);
			op2->index=realloc(op2->index,(strlen(op2->index)+2));
			printf("%s\n",(char *)op2->index);
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
			printf("%s\n",(char *)op2->index);
			op1->index=op2->index;
			return EVERYTHINGSOKAY;	
		case 16://L->E,E
			free(op1->index);
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
						if(op1->index!=NULL) free(op1->index);
						return SEMANTIC_ERR;
					}
					else 
						if(((T_VarData *) ((Hitem *) tmp)->data)->is_def==false)
						{
							fprintf(stderr,"premenna %s nebola definovana\n",(char *) op1->index);
							if(op1->index!=NULL) free(op1->index);
							return SEMANTIC_ERR;
						}
				//generate instruction
				op1->d_type=((T_VarData *) ((Hitem *) tmp)->data)->type;
				return EVERYTHINGSOKAY;				
				}
				//generate instruction
				//generate instruction
				op1->d_type=(op1->type)-16;	
				return EVERYTHINGSOKAY;
			}
			
			
			
	}
	printf("semantika ok");
  return EVERYTHINGSOKAY;
}
