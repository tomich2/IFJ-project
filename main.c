// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Filip Listiak

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "main.h"


TOKEN *get_token ()
{
        int i;

        if (isspace(c)!=0)
        {
            while (isspace(c)!=0 && c!=EOF) c=fgetc(fp);
        }
        if (c==EOF) return NULL;
        if (c=='{') //preskakovanie komentarov
        {
            while ((c=fgetc(fp))!='}' && (c!=EOF));
        }

        i=0;
        switch (c)
        {
            case 'A'...'Z':
                case 'a'...'z':
                    case '_':
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

            case '0'...'9':
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
                else if (strstr(token->mem,".")==NULL)
                {
                    token->mem=string_implementation('\0',i+1,token->mem);
                    token->identity = 2;
                    return token;
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                token->identity = 3;
                return token;
            }

            case '+':
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

            case ':':
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
                    c=fgetc(fp);
                }
                token->mem=string_implementation('\0',i+1,token->mem);
                //c=fgetc(fp);
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










        /*i=0;
        do
        {
            //if (i==0 && isdigit(c)==0 && isalpha(c)==0 && c!='_') break;
            if (isdigit(token.lexem[i-1])==0  && isalpha(token.lexem[i-1])==0 && token.lexem[i-1]!='_' &&  (isdigit(c)!=0  || isalpha(c)!=0 || c=='_' ) && i>0) break;
            token.lexem[i]=c;
            i++;
        }
        while ((isdigit(c=fgetc(fp))!=0  || isalpha(c)!=0 || c=='_' ) && (c!=EOF));

        token.lexem[i+1]='\0';

        if (c==EOF) return NULL;

        for (int j=0;j<i;j++) printf ("%c",token.lexem[j]);
        printf ("\n");
        return &token; */

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
            int k=1;
            while (key_words[i]!=',')
            {
                i++;
                if (key_words[i]==key[k])
                {
                    p++;
                    k++;
                }
                else break;
            }
            if (p+1==(strlen(key)) && (k-p)==1) return 1;
        }
    }
    return 0;
}

char *string_implementation (char c,int i,char *mem)
{
    if (i%(NATIVE_CHAR-1)==0 && i>0) mem=allocate_string(i,mem);
    mem[i]=c;
    return mem;
}

char *allocate_string (int i,char *mem)
{
    char *mem_realloc;

    if (i%(NATIVE_CHAR-1)==0 && i>0)
    {
        mem_realloc=(char*)realloc(((char*)mem),(sizeof(char)*NATIVE_CHAR*k));
        if (mem_realloc==NULL)
        {
            fprintf(stderr,"Allocation failed");
            exit (99);
        }
        k++;
        mem=mem_realloc;
    }
    return mem;
}

char *first_allocation ()
{
    token->mem=(char*) malloc(sizeof(char)*NATIVE_CHAR);
    k=2;
    if (token->mem==NULL)
    {
        fprintf(stderr,"Allocation failed");
        exit (99);
    }
    return token->mem;
}

int open_file (char *filename,int argc)
{
    if (argc!=2)
    {
        fprintf (stderr,"Argument error\n");
        return 99;
    }
    fp = fopen (filename,"r");
    if (fp==NULL)
    {
        fprintf (stderr,"File not found\n");
        exit (99);
    }
    return 0;
}

void close_file ()
{
    fclose(fp);
}

int main (int argc, char **argv)
{
    open_file(argv[1],argc);
    c=fgetc(fp);

    token=malloc(sizeof(TOKEN));
    if (token==NULL)
    {
        fprintf(stderr,"Allocation failed");
        exit (99);
    }

    while ((token=get_token())!=NULL)
    {
        printf ("%s - %d\n",token->mem,token->identity);
        free (token->mem);
        if (token==NULL) break;

    }
    free (token);
    close_file (argv[1]);
    return 0;
}
