// IFJ14, projekt do predmetu IFJ pre 2BIT 2014/2015 //
/////// Autor: Jan Profant
///////        Filip Badan
///////        Michal Chomo
///////        Tomas Chomo
///////        Findo

#ifndef IAL_H
#define IAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum types_tab { FUNCTION, IDENTIFIER, LOOP } types;

typedef struct T_TabItem Titem;
struct T_TabItem {
	int dtype;
	int decl;
	};

//definicia datoveho typu Hitem - zaznam v tabulke
typedef struct htab_item Hitem;
//datova struktura zaznamu v tabulke htab_listitem
struct htab_item {
	char *key;
	types type;
	void *data;
	Hitem *next;
	};

//definicia datoveho typu tabulka
typedef struct htab htab_t;
//struktura tabulky htab
struct htab {
	unsigned long htab_size;
	Hitem *ptrs[];
	//int range;
	    };
//funkcie pre pracu s tabulkou
unsigned int hash_function(const char *str, unsigned long htab_size); //hashovacia funkcia
htab_t *htab_init(unsigned long size); // inicialiacia tabulky -treba zvolit vhodnu velkost
void *htab_search(htab_t *t,char *key); //vyhladavanie v tabulke- vrati data najdeneho prvku alebo NULL ak nenajde
int htab_new(htab_t *t,char *key,types type, void *data, size_t data_size); //novy prvok
//key - identifikator, type - funkcia premenna navestie viz enum types, data- void pointer cokolvek, data_size - velkost priadnych dat
void htab_free(htab_t *t); //uvolnenie celej tabulky vraten htab_clear
void htab_clear(htab_t *t); //uvolnenie vsetkych prvkov tabulky

int length_func(char *str);
char * copy_func (char *str,unsigned int i, unsigned int n);
unsigned int find_func (char *str,char *search_str);
char * sort_func (char * str);
void func_Sift_Down(char * str,int Left,int Right);
#endif
	
