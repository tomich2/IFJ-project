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
                            token->mem=string_implementation(c,i,token->mem);
                        }
                        c=fgetc(fp);
                        while ((c>='A' && c<='Z') || (c>='a' && c<='z') || (c=='_') || (c>='0' && c<='9'))
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            c=fgetc(fp);
                        }
                        token->mem=string_implementation('\0',i+1,token->mem);
                        if (is_key_word(token->mem)==0) token->identity=1;
                        else token->identity=6;
                        return token;
                    }

            case '0'...'9':                                                                     // je to cislo
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                c=fgetc(fp);
                while (c>='0' && c<='9')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    c=fgetc(fp);
                }
                if (c=='.')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    c=fgetc(fp);
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        c=fgetc(fp);
                    }
                    switch (c)
                    {
                        case 'e':
                        case 'E':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            c=fgetc(fp);
                            while ((c)>='0' && c<='9')
                            {
                                i++;
                                token->mem=string_implementation(c,i,token->mem);
                                c=fgetc(fp);
                            }
                            break;
                        }
                        case '+':
                        case '-':
                        {
                            i++;
                            token->mem=string_implementation(c,i,token->mem);
                            c=fgetc(fp);
                            if (c=='e' || c=='E')
                            {
                                c=fgetc(fp);
                                while ((c)>='0' && c<='9')
                                {
                                    i++;
                                    token->mem=string_implementation(c,i,token->mem);
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
                    c=fgetc(fp);
                    while ((c)>='0' && c<='9')
                    {
                        i++;
                        token->mem=string_implementation(c,i,token->mem);
                        c=fgetc(fp);
                    }
                }
                else if ((strstr(token->mem,"."))==NULL)
                {
                    token->mem=string_implementation('\0',i+1,token->mem);
                    token->identity = 2;
                    return token;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 3;
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
                                token->mem=string_implementation(c,i,token->mem);
                            }
                            token->mem=string_implementation('\0',i+1,token->mem);
                            c=fgetc(fp);
                            token->identity = 7;
                            return token;
                        }

            case ':':                                                                                   // je to :, moze sa jednat o priradenie
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                c=fgetc(fp);
                if (c=='=')                                                                             // jedna sa o priradenie
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    c=fgetc(fp);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 7;
                return token;
            }

            case ';':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                c=fgetc(fp);
                token->identity = 7;
                return token;
            }

            case '>':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                c=fgetc(fp);
                if (c=='=')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 7;
                return token;
            }

            case '<':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                c=fgetc(fp);
                if (c=='>')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                }
                if (c=='=')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 7;
                return token;
            }

            case '(':
            case ')':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                c=fgetc(fp);
                token->identity = 7;
                return token;
            }

            case '\'':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                }
                c=fgetc(fp);
                while (c!='\'' && c!=EOF)
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                    c=fgetc(fp);
                }
                if (c=='\'')
                {
                    i++;
                    token->mem=string_implementation(c,i,token->mem);
                }
                else return NULL;
                c=fgetc(fp);
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 5;
                return token;
            }

            case '.':
            {
                if (i==0)
                {
                    token->mem=first_allocation ();
                    token->mem=string_implementation(c,i,token->mem);
                    c=fgetc(fp);
                    token->mem=string_implementation('\0',i+1,token->mem);
                    token->identity = 7;
                    return token;
                }
            }
        }

    return NULL;
}

int is_key_word (char *key)
{
    unsigned int p=0;
    for (unsigned int i=1;i<=strlen(key_words);i++)
    {
        if (key[0]==key_words[i] && key_words[i-1]==',')
        {
            p=0;
            int j=1;
            while (key_words[i]!=',')
            {
                i++;
                if (key_words[i]==key[j])
                {
                    p++;
                    j++;
                }
                else break;
            }
            if (p+1==(strlen(key)) && (j-p)==1) return 1;                                               // vrat 1 ak si nasiel identifikator v klucovych slovach
        }
    }
    return 0;                                                                                           // nenasiel si nic :(
}

int open_file (char *filename,int argc)
{
    if (argc!=2)
    {
        Error(99);
    }
    fp = fopen (filename,"r");
    if (fp==NULL)
    {
        Error(99);
    }
    return 0;
}

void close_file ()
{
    fclose(fp);
}
