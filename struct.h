#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum{tINTEGER, tSTRING, tREAL, tBOOLEAN, tVAR, tERR=-1}T_vartype;

typedef enum
{
	/*ADRES*/
	I_LABEL,
	I_RETURN,
	I_CALL,
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
I_READ
//doplnit co treba

}InstName;



typedef struct Instruction
{
	InstName Iname;
	struct Variable *a;
	struct Variable *b;
	char *res;
}tInstruction;




typedef struct
{
		int i;
		double r;
		char *s;
		//string s;
}VariableData;



typedef struct Variable{

	VariableData data;
	T_vartype type;

}Variable;





#endif
