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

unsigned int hash_function(const char *str, unsigned long htab_size) 
{ 
 unsigned int h=0; 
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
 return new; //vracia ukazovatel na tabulku

}

void *htab_search(htab_t *t,char *key) 
{
 unsigned int hash=hash_function(key,t->htab_size);
 Hitem *item=t->ptrs[hash]; //ziskanie umiestnenia pomocou hash funkcie

 while(item!=NULL) 
	{ //prechadzanie zoznamu na danom indexe tabulky
	 if (strcmp(key,item->key)==0) return item->data; //ak najde odpovedajuci zaznam vrati ukazovatel nan
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
	 return -1;
	}
 strcpy(new->key,key); //zkopirovanie retazca do naalokovanej pameti
 
 new->data=(void *) malloc(data_size);
 if (new->data==NULL) 
 	{
	 free(new->key);
	 free(new);
	 printf("Malloc error!\n");
	 return -1;
	}
 memcpy(new->data,data,data_size);
 new->type=type;
 new->next=NULL; // inicializacia dalsieho zaznamu

 unsigned int hash=hash_function(key,t->htab_size);
 if (t->ptrs[hash]==NULL) t->ptrs[hash]=new;
 else 
	{
	 Hitem *item=t->ptrs[hash];
	 while(item->next!=NULL) item=item->next;
	 item->next=new; //pripojenie polozky do tabulky 
	}
 return 0;
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
