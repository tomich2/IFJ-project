#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


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
		double r;
		char *s;
		//string s;
}VariableData;
	 
typedef enum 
{
		VAR_Bool,
		VAR_Int,
		VAR_Real,
		VAR_String
		VAR_Var		//ked bude typu VAR_Var tak v data.s bude nazov, nie obycajny string
}VariableType;
	 


typedef struct Variable{

	VariableData data;
	VariableType type;
	
}Variable;





#endif
