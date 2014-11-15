// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum ERR {  LEXICAL_ERR=1,
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
