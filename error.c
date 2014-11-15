// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "error.h"

void Error(int p)
{
    switch (p)
    {
        case 1:
        {
            fprintf(stderr,"Chyba programu vramci lexikalnej analyzy.\n");
            exit (LEXICAL_ERR);
        }
        case 2:
        {
            fprintf(stderr,"Chyba programu vramci syntaktickej analyzy.\n");
            exit (SYNTAX_ERR);
        }
        case 3:
        {
            fprintf(stderr,"Chyba programu vramci semantickej analyzy, nedefinovana funkcia/premenna, pokus o redefiniciu funkcie/premennej.\n");
            exit (SEMANTIC_ERR);
        }
        case 4:
        {
            fprintf(stderr,"Semanticka chyba typovej kompatibility v aritmetickych, retazcovych a relacnych vyrazoch, popr. zly pocet alebo typ parametrov pri volani funkcie.\n");
            exit (EXPRESSION_ERR);
        }
        case 5:
        {
            fprintf(stderr,"Ostatne semanticke chyby.\n");
            exit (OTHER_SEM_ERR);
        }
        case 6:
        {
            fprintf(stderr,"Behova chyba pri nacitani hodnoty zo vstupu.\n");
            exit (INPUT_ERR);
        }
        case 7:
        {
            fprintf (stderr,"Behova chyba pri praci s neinicializovanou premennou.\n");
            exit (UNINICIALIZED_ERR);
        }
        case 8:
        {
            fprintf(stderr,"Behova chyba delenia nulou.\n");
            exit (ZERO_DIV_ERR);
        }
        case 9:
        {
            fprintf(stderr,"Ostatne behove chyby.\n");
            exit (AND_OTHERS_ERR);
        }
        case 99:
        {
            fprintf(stderr,"Interna chyba interpretu.\n");
            exit (INTERN_INTERPRETATION_ERR);
        }
    }
}
