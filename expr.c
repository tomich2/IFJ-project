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
      // {/* f */Q,  Q,  Q,  Q,  Q,  Q,  Q,   Q,   Q,   Q,  Q,  Q,  Q,  E,  Q,  Q},
	 {/* , */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  R,  L,  R,  Q},
	 {/* ( */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  L,  L,  E,  Q},
	 {/* ) */R,  R,  R,  R,  R,  R,  R,   R,   R,   R,  Q,  Q,  R,  Q,  R,  R},
	 {/* $ */L,  L,  L,  L,  L,  L,  L,   L,   L,   L,  L,  L,  Q,  L,  Q,  Q}
	};
oprs converttooprs(identita id)
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
	case DtChar:
        case DtReal:
	case BooleanTrue:
	case BooleanFalse:
	case DtString:
	    return ETerm;
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
int Reduction(Stack *stack)
{
  int rule;
  T_ParserItem tmp;
	tmp=*(T_ParserItem*) (top(stack));
	pop(stack);
	if (tmp.type==0) //mame neterminal 
		{
		 if(tmp.value.nonterm.type!=Exp) return -1; //ak to nie je E -chyba
		 tmp= *(T_ParserItem *) top(stack); //novy pop
		 pop(stack);
		 if(tmp.type==0) return -1; //ak to je neterminal chyba (ocakavame ciarku alebo operator)
		 if(converttooprs(tmp.value.term)==EComma) //ak je to ciarka - mame E,
			{
			 tmp= *(T_ParserItem *) top(stack); // novy pop
			 pop(stack);
			 if(tmp.type==0) //ak to je neterminal co ocakavame
				{
				 if(tmp.value.nonterm.type==handle) return -1; //ak je to handle -chyba
				 tmp= *(T_ParserItem *) top(stack); //novy pop
			 	 pop(stack);
				 if(tmp.type==1) return -1; //ak je to terminal -chyba
				 if(tmp.value.nonterm.type==handle)	return 2; // ak sme dostali handle co ocakavame redukujeme na LExp -> Exp, (L)Exp
				 else return -1;			  // ak to nie je handle -chyba
				}
			 else return -1;
			}
		 if(converttooprs(tmp.value.term)<=9) //ak je to operator - mame E op
			{
			 tmp=*(T_ParserItem *) top(stack); // novy pop
                         pop(stack);
                         if(tmp.type==0) //ak to je neterminal co ocakavame
                                {
                                 if(tmp.value.nonterm.type!=1) return -1; //ak to nie je E -chyba
                                 tmp= *(T_ParserItem *) top(stack); //novy pop
                                 pop(stack);
                                 if(tmp.type==1) return -1; //ak je to terminal -chyba
                                 if(tmp.value.nonterm.type==handle) return 1; // ak sme dostali handle co ocakavame redukujeme na Exp -> Exp op Exp
                                 else return -1;                          // ak to nie je handle -chyba
                                }
                         else return -1;

			}
		 return -1;
		}
	else 
		{
		 if((converttooprs(tmp.value.term)==EId) || (converttooprs(tmp.value.term)==ETerm)) 
			{
		 	 tmp=*(T_ParserItem *) top(stack); //novy pop
		 	 pop(stack);
		 	 if(tmp.type==1) return -1; //ak je to terminal -chyba
		 	 if(tmp.value.nonterm.type==handle)	return 1; // ak sme dostali handle co ocakavame redukujeme na Exp -> term
		 	 else return -1;			  // ak to nie je handle -chyba
			}
		 if(converttooprs(tmp.value.term)==ERpar) // mame pravu zatvorku
			{
		 	 tmp= *(T_ParserItem *) top(stack); //novy pop
		 	 pop(stack);
		 	 if(tmp.type==1) // mame terminal 
				{
				 if(converttooprs(tmp.value.term)!=ELpar) return -1; // ak to nie je lava zatvorka -chyba
				 else
				 	{
					 tmp= *(T_ParserItem *) top(stack); //novy pop
			 	 	 pop(stack);
		 	 		 if(tmp.type==0) return -1; //ak je to neterminal -chyba
				 	 if(tmp.value.nonterm.type!=EId)	return -1; // ak sme nedostali identifikator funkcie -chyba
		 	 		 tmp= *(T_ParserItem *) top(stack); //novy pop
				 	 pop(stack);
				 	 if(tmp.type==1) return -1; //ak je to terminal -chyba
				 	 if(tmp.value.nonterm.type==handle)	return 1; // ak sme dostali handle co ocakavame redukujeme na Exp->f()
				 	 else return -1;			  // ak to nie je handle -chyba
					}
				} 
			 else // mame neterminal
				{
				  tmp= *(T_ParserItem *) top(stack); //novy pop
                                  pop(stack);
				  if(converttooprs(tmp.value.term)!=ELpar) return -1; // ak to nie je lava zatvorka -chyba
                                  else
                                        {
                                         tmp= *(T_ParserItem *) top(stack); //novy pop
                                         pop(stack);
                                         if(tmp.type==0) //ak je to neterminal 
						{
						 if (tmp.value.nonterm.type==handle) return 1; // a je to handle tak redukujeme E->(E) alebo E->(L) 
					 	 else return -1;
						}
					 else //ak to je terminal
						{
						 
                                         	 if(converttooprs(tmp.value.term)!=EId)    return -1; // ak sme nedostali identifikator funkcie -chyba
						 
                                         	 tmp= *(T_ParserItem *) top(stack); //novy pop
                                         	 pop(stack);
                                         	 if(tmp.type==1) return -1; //ak je to terminal -chyba
                                         	 if(tmp.value.nonterm.type==handle) return 1; // ak sme dostali handle co ocakavame redukujeme na Exp->f(E) alebo Exp -> f(L)
						 else return -1;  // ak to nie je handle -chyba
						}
                                        }


				}
			}
		
		}
			     
  return 0;
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

int ExprParse()
{
  Stack stack;
  init(&stack, sizeof(T_ParserItem));
  int reduct;
  T_ParserItem in;
  T_ParserItem *top;
  TOKEN *lexem;
  in.type=TERMINAL;
  in.value.term=EEnd;
  push(&stack, &in, -1);

  if ((lexem=get_token())!=NULL) token=lexem;
  else token->identity=EEnd;
   free (token->mem);
  token->mem=NULL;
  top=GetTerm(&stack, 0);
  while ((converttooprs(top->value.term)!=EEnd) || (converttooprs(token->identity)!=EEnd))
	{
	 if (lexem!=NULL) token=lexem;
	 else token->identity=EEnd;
	 top=GetTerm(&stack, 0);
	// printf ("%s - %d\n",token->mem,token->identity);
	 switch (GetRule(converttooprs(top->value.term),converttooprs(token->identity)))
		{
		 case E: 
			 in.type=TERMINAL;
			 in.value.term=token->identity;
			 push(&stack, &in , -1);
			 if ((lexem=get_token())!=NULL) token=lexem;
	 		 else token->identity=EEnd;

			 free (token->mem);
			 token->mem=NULL;
			 top=GetTerm(&stack, 0);
			 break;
		 case L: 
			  top=GetTerm(&stack, 1);
			 in.type=TERMINAL;
			 in.value.term=token->identity;
			 push(&stack, &in , -1);
			 //free (token->mem);
			 if ((lexem=get_token())!=NULL) token=lexem;
	 		 else token->identity=EEnd;
			
			 free (token->mem);
			 token->mem=NULL;
  			
			 top=GetTerm(&stack, 0);
			 break;
		 case R:
			 if ((reduct=Reduction(&stack))>0) 
				{
				  in.type=NONTERMINAL;
			 	  in.value.nonterm.type=reduct;
				 // printf("redukujeme zasobnik na %i\n",reduct);
			 	  push(&stack, &in , -1);
				  top=GetTerm(&stack, 0);
				}
			 else 
				{
				  printf("chyba pri redukcii celkovo\n");
				 // free (token->mem);
				  S_erase(&stack);
  			 	  return 0; 
				}
			 break;
		 case Q: 
			 printf("chyba pri redukcii tabulka\n");
			// free (token->mem);
			 S_erase(&stack);
  			 return 0; 
		}

	}
S_erase(&stack);
return 0;
}
