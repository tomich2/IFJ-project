// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexical.h"
#include "lexstring.h"
#include "error.h"
#include "stack.h"
#include "parser.h"


int main (int argc, char **argv)
{
  int test;
    open_file(argv[1],argc);
    c=fgetc(fp);
    token=malloc(sizeof(TOKEN));
    if (token==NULL)
    {
        free(token);
        close_file();
        Error(99);
    }
    test=top_down();
    printf("Syntakticka analyza v poriadku. Hura do pice %d \n", test);
    free(token);
    close_file();
    return 0;
}
