// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "error.h"
#include "ial.h"
#include "garbage.h"

int length_func (char *str)
{
    int i=0;
    while (str[i]!='\0') i++;
    return i;
}


char * copy_func (char *str,unsigned int i, unsigned int n)
{
    if (i+n-1>(strlen(str))) return NULL;
    char * x=malloc(sizeof(char)*n+1*(sizeof(char)));
    x[n]='\0';
    InsertFirst(LGar,x);
    int p=0;
    for (unsigned int j=i-1;j<i+n-1;j++)
    {
        x[p]=str[j];
        if (x[p]=='\0') return x;
        p++;
    }
    return x;
}


unsigned int find_func (char *str,char *search_str)
{
    int length=strlen(search_str);
    int str_len=((int)strlen(str));
    if (length>str_len) return 0;
    int array[length];
    unsigned indicator=0;
    int i;
    for (i=0;i<length-1;i++)
    {
        array[i]=length-i-1;

        if (str[i]==str[length])
            indicator=array[i];
    }
    if (indicator==0) array[length-1]=length;
    else array[length-1]=indicator;

    i=length-1;
    int j;
    int copy_i=i;
    while (1)
    {
        j=length-1;
        if (search_str[length-1]==str[i])
        {
            copy_i=i;
            i--;
            j--;
            while (j>=0 && search_str[j]==str[i])
            {
                i--;
                j--;
            }
            if (j==-1) return i+2;
            j=length-1;
            while (j>=0 && search_str[j]!=str[i]) j--;
            i=copy_i;
            if (j==-1) i=i+array[length-1];
            else i=i+array[j];
            if (i>=str_len) return 0;
        }
        if (str[i]=='\0') return 0;
        i++;
        if (str[i]=='\0') return 0;
    }
}

void func_Sift_Down(char * str, int Left, int Right)
{
    int i,j;
    char p;
    bool Cont;
    i=Left;
    j=2*i;
    p=str[i-1];
    Cont=(j<Right) ? 1 : 0;
    while (Cont)
    {
        if (j<Right)
        {
            if (str[j-1]<str[j])
            {
                j++;
            }

        }
        if (p>=str[j-1])
        {
            Cont=false;
        }

        else
        {
            str[i-1]=str[j-1];
            i=j;
            j=2*i;
            Cont=(j<Right) ? 1 : 0;
        }

    }
    str[i-1]=p;
}

char * sort_func (char * str1)
{
    int N=strlen(str1);
    char * str;
    str=malloc(sizeof(char)*N+1*(sizeof(char)));
    str[N]='\0';
    InsertFirst(LGar,str);
    memcpy(str,str1,N);
    int i, Left, Right,p;
    Left=N/2;
    Right=N;
    for (i=Left;i>0;i--)
    {
        func_Sift_Down(str,i,Right);
    }
    for (Right=N;Right>1;Right--)
    {
        p=str[0];
        str[0]=str[Right-1];
        str[Right-1]=p;
        func_Sift_Down(str,1,Right-1);
    }
    return str;
}


unsigned long hash_function(const char *str, unsigned long htab_size)
{
 unsigned long h=0;
 const unsigned char *p;
 for(p=(const unsigned char*)str; *p!='\0'; p++)
 h = 65599*h + *p;
 return h % htab_size;
}

//inicializuje tabulku
htab_t *htab_init(unsigned long size)
{

 htab_t *new;
 new=(htab_t *) calloc(1,sizeof(htab_t)+(sizeof(char *) * size)); //alokuje a inicializuje pamat - velkost struktury htab +
								  //velkost daneho pola podla parametru size
 if (new==NULL) return NULL; //kontrola navratovej hodnoty
 new->htab_size=size;
 for (unsigned int i=0;i!=new->htab_size;i++) //cyklus na prechadzanie tabulky
	new->ptrs[i]=NULL;
 return new; //vracia ukazovatel na tabulku

}

void *htab_search(htab_t *t,char *key)
{
 unsigned long hash=hash_function(key,t->htab_size);
 Hitem *item=t->ptrs[hash]; //ziskanie umiestnenia pomocou hash funkcie

 while(item!=NULL)
 { //prechadzanie zoznamu na danom indexe tabulky
	 if (strcmp(key,item->key)==0) return item;//->data; //ak najde odpovedajuci zaznam vrati ukazovatel nan
	 item=item->next;
 }
 return NULL;
}


int htab_new(htab_t *t,char *key,types type, void *data, size_t data_size)
{
 Hitem *new=NULL;
 new=(Hitem *)malloc(sizeof(Hitem)); //alokacia novej polozky
 if (new==NULL) return -1; // kontrola navratovej hodnoty malloc
 new=memset(new,0,sizeof(Hitem)); //inicializacia pameti
 new->key=(char *)malloc(sizeof(char)*(strlen(key)+1)); //alokacia daneho retazca
 if (new->key==NULL)
 { //kontrola navratovej hodnoty malloc
	free(new);
	return INTERN_INTERPRETATION_ERR;
 }
 strcpy(new->key,key); //zkopirovanie retazca do naalokovanej pameti

 new->data=(void *) malloc(data_size);
 if (new->data==NULL)
 {
	free(new->key);
	free(new);
	fprintf(stderr,"Malloc error!\n");
	return INTERN_INTERPRETATION_ERR;
 }
 memcpy(new->data,data,data_size);
 new->type=type;
 new->next=NULL; // inicializacia dalsieho zaznamu

 unsigned long hash=hash_function(key,t->htab_size);
 if (t->ptrs[hash]==NULL) t->ptrs[hash]=new;
 else
 {
	Hitem *item=t->ptrs[hash];

	while (item->next!=NULL)
	{
		if(strcmp(item->key,key)==0)
		{
			free(new->key);
			free(new->data);
			free(new);
			return SEMANTIC_ERR;
		}
		item=item->next;
	}
	if(strcmp(item->key,key)==0)
		{
			free(new->key);
			free(new->data);
			free(new);
			return SEMANTIC_ERR;
		}
	item->next=new; //pripojenie polozky do tabulky
 }
 return EVERYTHINGSOKAY;
}

//funkcia vymazava a dealokuje vsetky zaznamy tabulky
void htab_clear(htab_t *t)
{
 unsigned long i;
 Hitem *tmp;
 Hitem *next;
 for (i=0;i!=t->htab_size;i++) //cyklus na prechadzanie tabulky
	if ((next=t->ptrs[i])!=NULL)
	{
		//postupne uvolnuje polozky zoznamu
		while(next!=NULL)
		{
               		tmp=next;
               		next=next->next;
               		free(tmp->key); //uvolnovanie retazce
			free(tmp->data);
                	free(tmp); // uvolnovanie zaznamu
                }
		t->ptrs[i]=NULL;
	}
 return;
}

void htab_free(htab_t *t)
{
 htab_clear(t); //vola funkciu na uvolnenie vsetkych poloziek
 free(t); //uvolni celu tabulku
 return;
}
