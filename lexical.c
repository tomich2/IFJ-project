// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexical.h"
#include "lexstring.h"


//retazec klucovych slov
const char key_words [] = {",begin,boolean,do,else,end,false,find,forward,function,if,integer,readln,real,sort,string,then,true,var,while,write,"};
//

ERROR_MSG get_token ()
{
        int i;
        if (isspace(c)!=0)                                                                      // preskoc medzery
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        if (c==EOF)
        {
            token->identity=EndOfFile;
            return EVERYTHINGSOKAY;
        }
        if (c=='{')                                                                             //preskakovanie komentarov
        {
            while ((c=fgetc(fp))!='}' && (c!=EOF));
        }
        if (c=='}') c=fgetc(fp);
        if (isspace(c)!=0)                                                                      // preskoc medzery
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        i=0;
        switch (c)
        {
            case 'A'...'Z':
                case 'a'...'z':
                    case '_':                                                                   // konecny automat pre identifikator
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
                        c=fgetc(fp);
                        while ((c>='A' && c<='Z') || (c>='a' && c<='z') || (c=='_') || (c>='0' && c<='9'))
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
                        int Iden;
                        token->mem=string_implementation('\0',i+1,token->mem);
                        if (token->mem==NULL)
                        {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                        }
                        if ((Iden=is_key_word(token->mem))==0) token->identity=ID;
                        else token->identity=Iden;
                        return EVERYTHINGSOKAY;
                    }

            case '0'...'9':                                                                     // je to cislo
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
                if (c=='.')
                {
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
                    switch (c)
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
                    /*    case '+':
                        case '-':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL) return INTERN_INTERPRETATION_ERR;
                            c=fgetc(fp);
                            if (c=='e' || c=='E')
                            {
                                c=fgetc(fp);
                                while ((c)>='0' && c<='9')
                                {
                                    i++;
                                    token->mem=string_implementation(c,i,token->mem);
                                    if (token->mem==NULL) return INTERN_INTERPRETATION_ERR;
                                    c=fgetc(fp);
                                }
                            }
                            break;
                        }*/
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
                    if ((c!='+' || c!='-') &&  (c<'0' || c>'9'))
                    {
                        printf("pica");
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
                }
                else
                {
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    if ((strchr(token->mem,'.'))==NULL)
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

            case ';':
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

            case '=':
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

            case ',':
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

            case '>':
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
                if (c=='=')
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

            case '<':
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
                if (c=='>')
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
                if (c=='=')
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

            case '(':
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

            case '\'':
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
               /* while (1)
                {
                    c=fgetc(fp);
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
                        if (parity % 2 == 1)
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
                            }
                            break;
                        }
                        if ((c!='#' && c!='\''))
                        {
                            end=true;
                            break;
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
                    if (end)
                    {
                        break;
                    }

                }*/

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
                        if (c<32)
                        {
                            free(token->mem);
                            return LEXICAL_ERR;
                        }
                        if (c==')' || c==',' || c==';')
                        {
                            end=true;
                            break;
                        }
                        else
                        {
                            if (c=='\'') parity++;
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL)
                            {
                                free(token->mem);
                                return INTERN_INTERPRETATION_ERR;
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

            case '.':
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

           /* case '#':
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
                    if (c<'0' || c>'9')
                    {
                        free(token->mem);
                        return LEXICAL_ERR;
                    }
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
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL)
                    {
                        free(token->mem);
                        return INTERN_INTERPRETATION_ERR;
                    }
                    return EVERYTHINGSOKAY;
                }*/


            default:
            {
                //token->identity=EndOfFile;
                //free(token->mem);
                return LEXICAL_ERR;
            }
        }

    token->identity=EndOfFile;
    return EVERYTHINGSOKAY;
}

int CaseInsensitiveCharCmp (char c1, char c2)
{
    c1=toupper(c1);
    c2=toupper(c2);
    if (c1==c2) return 0;
    else return -1;
}

int is_key_word (char *key)
{
    unsigned int p=0;
    for (unsigned int i=1;i<=strlen(key_words);i++)
    {
        if (CaseInsensitiveCharCmp(key[0],key_words[i])==0 && key_words[i-1]==',')
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
                switch (i)
                {
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

void close_file ()
{
    fclose(fp);
}
