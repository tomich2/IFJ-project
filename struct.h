#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

typedef enum{tINTEGER, tSTRING, tREAL, tBOOLEAN, tVAR, tERR=-1}T_vartype;

typedef enum
{
	/*ADRES*/
	I_LABEL,
	I_RETURN,
	I_CALL,
	I_GOTO,
	/*ARITMETIKA*/
	I_ADD,
	I_SUB,
	I_MUL,
	I_DIV,
	I_MOD,
	/*COMPARING*/
	I_LESS_CMP,
	I_GR_CMP,
	I_LESS_EQUAL_CMP,
	I_GR_EQUAL_CMP,
	I_EQUAL_CMP,
	I_NOT_EQUAL_CMP,

I_ASSIGN,
I_PRINT,
I_READ,

I_PUSH,
I_POP,

I_COPY,
I_FIND,
I_LENGTH,
I_SORT
//doplnit co treba

}InstName;



typedef struct Instruction
{
	InstName Iname;
	struct Variable *a;
	struct Variable *b;
	void *res;
}tInstruction;




	/*************VALUEEEE********************/

typedef struct
{
		int i;
		int label;
		double r;
		char *s;
		bool b;
		//string s;
}VariableData;


typedef struct Variable{

	VariableData data;
	T_vartype type;

}Variable;





#endif
