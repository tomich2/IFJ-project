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

//retazec klucovych slov
const char key_words [] = {",begin,boolean,do,else,end,false,find,forward,function,if,integer,readln,real,sort,string,then,true,var,while,write,"};
//

TOKEN *get_token ()
{
        int i;

        if (isspace(c)!=0)                                                                      // preskoc medzery
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        if (c==EOF) return NULL;
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
                            if (token->mem==NULL) return NULL;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL) return NULL;
                        }
                        c=fgetc(fp);
                        while ((c>='A' && c<='Z') || (c>='a' && c<='z') || (c=='_') || (c>='0' && c<='9'))
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL) return NULL;
                            c=fgetc(fp);
                        }
                        int Iden;
                        token->mem=string_implementation('\0',i+1,token->mem);
                        if (token->mem==NULL) return NULL;
                        if ((Iden=is_key_word(token->mem))==0) token->identity=1;
                        else token->identity=Iden;
                        return token;
                    }

            case '0'...'9':                                                                     // je to cislo
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                c=fgetc(fp);
                while (c>='0' && c<='9')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                }
                if (c=='.')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL) return NULL;
                        c=fgetc(fp);
                    }
                    switch (c)
                    {
                        case 'e':
                        case 'E':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL) return NULL;
                            c=fgetc(fp);
                            while ((c)>='0' && c<='9')
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL) return NULL;
                                c=fgetc(fp);
                            }
                            break;
                        }
                        case '+':
                        case '-':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            if (token->mem==NULL) return NULL;
                            c=fgetc(fp);
                            if (c=='e' || c=='E')
                            {
                                c=fgetc(fp);
                                while ((c)>='0' && c<='9')
                                {
                                    i++;
                                    token->mem=string_implementation(c,i,token->mem);
                                    if (token->mem==NULL) return NULL;
                                    c=fgetc(fp);
                                }
                            }
                            break;
                        }
                    }
                }
                if (c=='e' || c=='E')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        if (token->mem==NULL) return NULL;
                        c=fgetc(fp);
                    }
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                else
                {
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL) return NULL;
                    if ((strchr(token->mem,'.'))==NULL)
                    {
                        token->mem=string_implementation('\0',i+1,token->mem);
                        if (token->mem==NULL) return NULL;
                        token->identity = 37;
                        return token;
                    }
                }
                if (token->mem[i+1]!=0) token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                token->identity = 39;
                return token;
            }

            case '+':                                                                                   // je to numericky operator
                case '-':
                    case '*':
                        case '/':
                        {
                            if (i==0)
                            {
                                token->mem=first_allocation ();
                                if (token->mem==NULL) return NULL;
                                token->mem=string_implementation(c,i,token->mem);
                                if (token->mem==NULL) return NULL;
                            }
                            token->mem=string_implementation('\0',i+1,token->mem);
                            if (token->mem==NULL) return NULL;
                            if (c=='+') token->identity=22;
                            if (c=='-') token->identity=23;
                            if (c=='*') token->identity=24;
                            if (c=='/') token->identity=25;
                            c=fgetc(fp);
                            return token;
                        }

            case ':':                                                                                   // je to :, moze sa jednat o priradenie
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->identity=26;
                c=fgetc(fp);
                if (c=='=')                                                                             // jedna sa o priradenie
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                    token->identity=27;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                return token;
            }

            case ';':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                c=fgetc(fp);
                token->identity = 28;
                return token;
            }

            case '=':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                c=fgetc(fp);
                token->identity = 33;
                return token;
            }

            case ',':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                c=fgetc(fp);
                token->identity = 43;
                return token;
            }

            case '>':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->identity=29;
                c=fgetc(fp);
                if (c=='=')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    token->identity = 31;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                c=fgetc(fp);
                return token;
            }

            case '<':
            {
                if (i==0)
                {
                    token->identity=30;
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                c=fgetc(fp);
                if (c=='>')
                {
                    i++;
                    token->identity=42;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                if (c=='=')
                {
                    i++;
                    token->identity=32;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                c=fgetc(fp);
                return token;
            }

            case '(':
            case ')':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                if (c=='(') token->identity=34;
                else token->identity=35;
                c=fgetc(fp);
                return token;
            }

            case '\'':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                c=fgetc(fp);
                while (c!='\'' && c!=EOF)
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                }
                if (c=='\'')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                }
                else return NULL;
                c=fgetc(fp);
                token->mem=string_implementation('\0',i+1,token->mem);
                if (token->mem==NULL) return NULL;
                token->identity = 41;
                return token;
            }

            case '.':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    if (token->mem==NULL) return NULL;
                    token->mem=string_implementation(c,i,token->mem);
                    if (token->mem==NULL) return NULL;
                    c=fgetc(fp);
                    token->mem=string_implementation('\0',i+1,token->mem);
                    if (token->mem==NULL) return NULL;
                    token->identity = 36;
                    return token;
                }
            }
        }

    return NULL;
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
                        return 2;
                    case 14:
                        return 3;
                    case 17:
                        return 4;
                    case 22:
                        return 5;
                    case 26:
                        return 6;
                    case 32:
                        return 7;
                    case 37:
                        return 8;
                    case 45:
                        return 9;
                    case 54:
                        return 10;
                    case 57:
                        return 11;
                    case 65:
                        return 12;
                    case 72:
                        return 13;
                    case 77:
                        return 14;
                    case 82:
                        return 15;
                    case 89:
                        return 16;
                    case 94:
                        return 17;
                    case 99:
                        return 18;
                    case 103:
                        return 19;
                    case 109:
                        return 20;
                    case 115:
                        return 21;
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
