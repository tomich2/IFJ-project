// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/*vycet typov*/
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
	/*COMPARING*/
	I_LESS_CMP,
	I_GR_CMP,
	I_LESS_EQUAL_CMP,
	I_GR_EQUAL_CMP,
	I_EQUAL_CMP,
	I_NOT_EQUAL_CMP,
	/*ostatne*/
	I_ASSIGN,
	I_PRINT,
	I_READ,
	I_PUSH,
	/*vstavane*/
	I_COPY,
	I_FIND,
	I_LENGTH,
	I_SORT

}InstName;


/*struktura instrukcie*/
typedef struct Instruction
{
	InstName Iname;
	struct Variable *a;
	struct Variable *b;
	char *res;
}tInstruction;



/*data*/
typedef struct
{
		int i;
		double r;
		char *s;
		bool b;
}VariableData;


typedef struct Variable{

	VariableData data;
	T_vartype type;

}Variable;





#endif
