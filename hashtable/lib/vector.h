#ifndef VECTOR_GUARD
#define VECTOR_GUARD

#include <stdlib.h>

#include "hash_table.h"
#include "data.h"

// a variable size vector of any type
typedef struct vector {
	valType *buff;
	size_t size;
	size_t capacity;
} vector;

vector* init_vec(size_t, valType*, size_t);
vector* append(vector*, valType);

#endif