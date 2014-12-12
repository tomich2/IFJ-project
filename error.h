// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //

/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak

#ifndef _ERROR_H
#define _ERROR_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

//
// Subor error.h obsahuje difinicu funkcie Error() a definiciu vyctoveho typu pre navratovy kod programu
//

typedef enum ERR {
                    EVERYTHINGSOKAY=0,
                    LEXICAL_ERR=1,
                    SYNTAX_ERR,
                    SEMANTIC_ERR,
                    EXPRESSION_ERR,
                    OTHER_SEM_ERR,
                    INPUT_ERR,
                    UNINICIALIZED_ERR,
                    ZERO_DIV_ERR,
                    AND_OTHERS_ERR,
                    INTERN_INTERPRETATION_ERR=99
                 }ERROR_MSG;

void Error(int p);

#endif
