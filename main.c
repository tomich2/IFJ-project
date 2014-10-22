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


int main (int argc, char **argv)
{
    open_file(argv[1],argc);
    c=fgetc(fp);

    token=malloc(sizeof(TOKEN));
    if (token==NULL)
    {
        close_file();
        Error(99);
    }

    TOKEN *lexem;
    while ((lexem=get_token())!=NULL)
    {
        if (lexem!=NULL) token=lexem;
        else break;
        printf ("%s - %d\n",token->mem,token->identity);
        free (token->mem);
    }
    free (token);
    close_file (argv[1]);
    return 0;
}
