#include <string.h>
#include <stdlib.h>
#include "vector.h"

/**
	*Inicializuje vektor
*/
void vectorInit(Vector *vec)
{
	vec->data = NULL;
	vec->size = 0;
	vec->capacity = 0;
}

void vectorAlloc(Vector *vec, uint32_t capacity)
{
	if(capacity <= vec->capacity)
	return;

	void *tmp;

	if(vec->capacity > 0)
		tmp = realloc(vec->data, vec->itemSize * capacity);
	
	else
		tmp = malloc(vec->itemSize * capacity);
	
	if(tmp == NULL)
	{
		//Error
		return;
	}
	
	vec->data=tmp;
	vec->end = vec->data + vec->size * vec->itemSize;
	vec->capacity = capacity;
	memset(vec->end, 0, (vec->capacity - vec->size) * vec->itemSize);
}
						


/**
	*Vrati aktualnu kapacitu vektora
*/
uint32_t vectorCapacity(const Vector *vec)
{
	return vec->capacity;
}


/**
	*Vrati ukazovatel na polozku na urcenom indexe
*/
void* vectorGet(Vector *vec, uint32_t index)
{
	//Err handling
	
	if(vec->size <= index)
	{	
		//Error
		return NULL;
	}		

	return vec->data + vec->itemSize * index;
}		

/**
	*Vrati ukazovatel na prvu polozku vo vektore
*/
void* vectorFirst(Vector *vec)
{
	if(vec->size == 0)
	{
		//Error
		return NULL;
	}
	return vec->data;
}
/**
	*Vrati ukazovatel na poslednu polozku vektoru
*/
void* vectorLast(Vector *vec)
{
	if(vec->size == 0)
	{
		//Error
		return NULL;
	}
	return vec->end - vec->itemSize;
}
	/**
int main(void)
{
return 0;
}
**/
