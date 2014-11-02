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
    
    /* test zasobnika ***************************** */
     char s1[]="Ahoj";
     char s2[]="Svet";

     char *s;

     Stack stack;
     init(&stack, 255);
     push(&stack,s1,-1);
     printf("vrchol je %s\n", s=top(&stack));
     pop(&stack);
     if (S_empty(&stack)) printf("zasobnik je prazdny\n");
     push(&stack,s1,strlen(s1));
     push(&stack,s2,strlen(s2));
     printf("vrchol je %s\n", s=top(&stack));
     pop(&stack);
     printf("vrchol je %s\n", s=top(&stack));
     pop(&stack);
     if (S_empty(&stack)) printf("zasobnik je prazdny\n");
     /* test zasobnika ***************************** */
    return 0;
}
