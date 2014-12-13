// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////         Filip Badan
///////         Michal Chomo
///////         Tomas Chomo
///////         Filip Listiak

#ifndef INTERPRET_H
#define INTERPRET_H

#include "list.h"

void interpretLoop(tListOfInstr *instList,t_varfunc_list *varList,t_lablist *lablist);
void print_my_string (char *str);
void printchar(unsigned char theChar);
char * convert_my_string (char *str);
char convertchar(unsigned char theChar);


#endif
