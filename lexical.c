// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //

/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexical.h"
#include "lexstring.h"
#include "ial.h"

//
// Subor lexical.c obsahuje vsetku implementaciu lexikalnej analyzy prekladaca a intepreta IFJ14
//

// retazec klucovych slov
const char key_words [] = {",begin,boolean,do,else,end,false,find,forward,function,if,integer,readln,real,sort,string,then,true,var,while,write,"};

// Funkcia get_token vracia spravu o chybe, resp. spravnom nacitani jedneho tokenu
// Pracuje s globalnou strukturou token

ERROR_MSG get_token ()
{
        int i;
        if (isspace(c)!=0)                                                                      // preskoc medzery
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        if (c==EOF)                                                                             // narazil si na koniec suboru, referuj o tom
        {
            token->identity=EndOfFile;
            return EVERYTHINGSOKAY;
        }
        if (c=='{')                                                                             //preskakovanie komentarov
        {
            do
            {
            c=fgetc(fp);
            }
            while (c!='}' && c!=EOF);
            if (c==EOF)
            {
                return LEXICAL_ERR;
            }
            else
            {
                c=fgetc(fp);
                while ((isspace(c)!=0 && c!=EOF) || (c=='{'))
                {
                    if (c=='{')
                    {
                        c=fgetc(fp);
                        while (c!='}' && c!=EOF)
                        {
                            c=fgetc(fp);
                        }
                    }
                    c=fgetc(fp);
                }
                if (c==EOF)
                {
                    token->identity=EndOfFile;
                    return EVERYTHINGSOKAY;
                }
            }
        }

        if (isspace(c)!=0)                                                                      // preskoc medzery
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        i=0;
        switch (c)                                                                              // KONECNY AUTOMAT pre TOKEN
        {
            case 'A'...'Z':                                                                     //
                case 'a'...'z':                                                                 //
                    case '_':                                                                   // konecny automat pre identifikator
                    {
                        if (i==0)                                                               // naalokuj a uloz mi prvy znak
                        {
                            token->mem=first_allocation ();
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                        }
                        c=fgetc(fp);
                        while ((c>='A' && c<='Z') || (c>='a' && c<='z') || (c=='_') || (c>='0' && c<='9'))
                        {                                                                       // a nacitavaj a ukladaj kym mas co
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            c=fgetc(fp);
                        }
                        int Iden;
                        token->mem=string_implementation('\0',i+1,token->mem);                  // ukonci korektne retazec
                        if (token->mem==NULL)
                        {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                        }
                        if ((Iden=is_key_word(token->mem))==0) token->identity=ID;              // je to identifikator alebo
                        else token->identity=Iden;                                              // klucove slovo<
                        return EVERYTHINGSOKAY;
                    }

            case '0'...'9':                                                                     // je to cislo
            {
                int bodka=0;
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                c=fgetc(fp);
                while (c>='0' && c<='9')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    c=fgetc(fp);
                }
                if (c=='.')                                                                     // je to desatinne cislo
                {
                    bodka=1;
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    c=fgetc(fp);
                    if (c<'0' || c>'9')
                    {
                        free(token->mem);
                        return LEXICAL_ERR;
                    }
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL)
                        {
                            free(token->mem);
                            return INTERN_INTERPRETATION_ERR;
                        }
                        c=fgetc(fp);
                    }
                    switch (c)                                                                  // nasiel si exponent
                    {
                        case 'e':
                        case 'E':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            c=fgetc(fp);
                            if (c=='+' || c=='-')
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                            }
                            else
                            {
                                if (c<'0' || c>'9')
                                {
                                    free(token->mem);
                                    return LEXICAL_ERR;
                                }
                            }
                            c=fgetc(fp);
                            if (c>'9' || c<'0')
                            {
                                free(token->mem);
                                return LEXICAL_ERR;
                            }
                            while ((c)>='0' && c<='9')
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                                c=fgetc(fp);
                            }
                            break;
                        }
                    }
                }
                if (c=='e' || c=='E')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    c=fgetc(fp);
                    if (!((c>'0' && c<'9') || (c=='+' || c=='-')))
                    {
                        free(token->mem);
                        return LEXICAL_ERR;
                    }
                    if (c=='+' || c=='-')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL)
                        {
                            free(token->mem);
                            return INTERN_INTERPRETATION_ERR;
                        }
                    }
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL)
                        {
                            free(token->mem);
                            return INTERN_INTERPRETATION_ERR;
                        }
                        c=fgetc(fp);
                    }
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    if (bodka==0)
                    {
                        token->identity=DtReal;
                        return EVERYTHINGSOKAY;
                    }
                }
                else
                {
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    if (bodka==0)
                    {

                        token->mem=string_implementation('\0',i+1,token->mem);
                        if (token->mem==NULL)
                        {
                            free(token->mem);
                            return INTERN_INTERPRETATION_ERR;
                        }
                        token->identity = DtInteger;

                        return EVERYTHINGSOKAY;
                    }
                }
                if (token->mem[i+1]!=0) token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                token->identity = DtReal;
                return EVERYTHINGSOKAY;
            }

            case '+':                                                                                   // je to numericky operator
                case '-':
                    case '*':
                        case '/':
                        {
                            if (i==0)
                            {
                                token->mem=first_allocation ();
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                            }
                            token->mem=string_implementation('\0',i+1,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            if (c=='+') token->identity=OpPlus;
                            if (c=='-') token->identity=OpMinus;
                            if (c=='*') token->identity=OpKrat;
                            if (c=='/') token->identity=OpDiv;
                            c=fgetc(fp);
                            return EVERYTHINGSOKAY;
                        }

            case ':':                                                                                   // je to :, moze sa jednat o priradenie
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->identity=OpDek;
                c=fgetc(fp);
                if (c=='=')                                                                             // jedna sa o priradenie
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    c=fgetc(fp);
                    token->identity=OpPrir;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                return EVERYTHINGSOKAY;
            }

            case ';':                                                                                   // je to bodkociarka (strednik)
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                c=fgetc(fp);
                token->identity = OpKonec;
                return EVERYTHINGSOKAY;
            }

            case '=':                                                                                   // operator rovnosti
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                c=fgetc(fp);
                token->identity = OpRovny;
                return EVERYTHINGSOKAY;
            }

            case ',':                                                                                   // ciarka
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                c=fgetc(fp);
                token->identity = OpCiarka;
                return EVERYTHINGSOKAY;
            }

            case '>':                                                                                   // operator porovnania - vacsi
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->identity=OpVacsi;
                c=fgetc(fp);
                if (c=='=')                                                                              // vacsi rovny
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->identity = OpVacsiR;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                c=fgetc(fp);
                return EVERYTHINGSOKAY;
            }

            case '<':                                                                                       // operator porovnania - mensi
            {
                if (i==0)
                {
                    token->identity=OpMensi;
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                c=fgetc(fp);
                if (c=='>')                                                                                 // nerovny
                {
                    i++;
                    token->identity=OpNerovny;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                if (c=='=')                                                                                 // mensi rovny
                {
                    i++;
                    token->identity=OpMensiR;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                c=fgetc(fp);
                return EVERYTHINGSOKAY;
            }

            case '(':                                                                                       // je to jedna zo zatvoriek
            case ')':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                if (c=='(') token->identity=OpLZat;
                else token->identity=OpPZat;
                c=fgetc(fp);
                return EVERYTHINGSOKAY;
            }

            case '\'':                                                                                      // retazcovy literal
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                }

                int parity=1;
                bool end=false;

                while (1)
                {

                    c=fgetc(fp);
                    if (c<32)
                    {
                        free(token->mem);
                        return LEXICAL_ERR;
                    }
                    if ((c==';' || c==')' || c==',' )) break;
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    if (c=='\'')
                    {
                        parity++;
                        c=fgetc(fp);

                        if (c==')' || c==',' || c==';' || isspace(c)!=0)
                        {
                            end=true;
                            break;
                        }
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL)
                        {
                            free(token->mem);
                            return INTERN_INTERPRETATION_ERR;
                        }
                        if (c=='#')
                        {
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            c=fgetc(fp);
                            int init = i;
                            while (c>='0' && c<='9')
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                                c=fgetc(fp);
                            }

                            char convert [i-init+1];
                            strncpy(convert,(token->mem+init+1),i-init);
                            convert[i-init]='\0';
                            if (c=='\'')
                            {
                                i++;
                                parity++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                                int numero = atoi(convert);
                                if (numero<=0 || numero>255)
                                {
                                    free(token->mem);
                                    return LEXICAL_ERR;
                                }

                            }
                            else
                            {
                                free(token->mem);
                                return LEXICAL_ERR;
                            }





                        }

                        else
                        {
                            if (c=='\'')
                            {

                                parity++;
                            }
                            else
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL)
                                {
                                    free(token->mem);
                                    return INTERN_INTERPRETATION_ERR;
                                }
                            }
                        }

                    }
                    if (end) break;
                }



                if (parity % 2 ==1)
                {
                    free(token->mem);
                    return LEXICAL_ERR;
                }
                //c=fgetc(fp);
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL)
                {
                    free(token->mem);
                    return INTERN_INTERPRETATION_ERR;
                }
                token->identity = DtString;
                return EVERYTHINGSOKAY;
            }

            case '.':                                                                                   // bodka
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    c=fgetc(fp);
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    token->identity = OpBodka;
                    return EVERYTHINGSOKAY;
                }
            }

            default:                                                                                    // neocakavany znak, vrat chybu
            {
                return LEXICAL_ERR;
            }
        }

    token->identity=EndOfFile;
    return EVERYTHINGSOKAY;
}

// Funkcia porovnava dva znaky kvoli INCASESENSITIVITE jazyka IFJ14

int CaseInsensitiveCharCmp (char c1, char c2)
{
    c1=toupper(c1);
    c2=toupper(c2);
    if (c1==c2) return 0;
    else return -1;
}

// Funkcia is_key_word zisti ci je retazec zadany klucom klucove slovo jazyka alebo nie

int is_key_word (char *key)
{
    unsigned int p=0;
    for (unsigned int i=1;i<=strlen(key_words);i++)
    {
        if (CaseInsensitiveCharCmp(key[0],key_words[i])==0 && key_words[i-1]==',')                  // prejdi cele pole znakov key_words
        {
            p=0;
            int j=1;
            while (key_words[i]!=',')
            {
                i++;
                if (CaseInsensitiveCharCmp(key_words[i],key[j])==0)
                {
                    p++;
                    j++;
                }
                else break;
            }
            if (p+1==(strlen(key)) && (j-p)==1 && ((strlen(key))>1))
            {
                switch (i)                                                                          // podla polohy ciarky rozhodni ktore klucove
                {                                                                                   // slovo to je
                    case 6:
                        return KwBegin;
                    case 14:
                        return KwBoolean;
                    case 17:
                        return KwDo;
                    case 22:
                        return KwElse;
                    case 26:
                        return KwEnd;
                    case 32:
                        return BooleanFalse;
                    case 37:
                        return KwFind;
                    case 45:
                        return KwForward;
                    case 54:
                        return KwFunction;
                    case 57:
                        return KwIf;
                    case 65:
                        return KwInteger;
                    case 72:
                        return KwReadln;
                    case 77:
                        return KwReal;
                    case 82:
                        return KwSort;
                    case 89:
                        return KwString;
                    case 94:
                        return KwThen;
                    case 99:
                        return BooleanTrue;
                    case 103:
                        return KwVar;
                    case 109:
                        return KwWhile;
                    case 115:
                        return KwWrite;
                }

            }
        }
    }
    return 0;                                                                                           // nenasiel si nic :(
}

// Funkcia open_file otvori subor, resp. vrati chybu

int open_file (char *filename,int argc)
{
    if (argc!=2)
    {
        Error(INTERN_INTERPRETATION_ERR);
    }
    fp = fopen (filename,"r");
    if (fp==NULL)
    {
        Error(INTERN_INTERPRETATION_ERR);
    }
    return 0;
}

// Funkcia close_file zatvori subor

void close_file ()
{
    fclose(fp);
}

