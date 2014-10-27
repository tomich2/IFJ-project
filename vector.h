#ifndef VECTOR_H
#define VECTOR_H

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	uint32_t size;
	uint32_t capacity;
	unsigned char *data;
	unsigned char *end;
	size_t itemSize;
	uint32_t count;
	
} Vector;

void vectorInit(Vector *v);
void vectorAlloc(Vector *vec, uint32_t capacity);
uint32_t vectorCapacity(const Vector *vec);
void* vectorGet(Vector *vec, uint32_t index);
void* vectorFirst(Vector *vec);
void* vectorLast(Vector *vec);

static inline uint32_t vectorSize(const Vector *vec)
{
	return vec->size;
}


#endif
