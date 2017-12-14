#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "vector.h"

// creates a new vector and returns a pointer to it
vector* init_vec(size_t init_capacity, valType *init_vals, size_t init_size) {
	printf("init_capacity %zu\n", init_capacity);
	printf("init_size %zu\n", init_size);
	if (init_vals != NULL) {
		assert(init_capacity >= init_size);
	}

	// allocate vector
	vector *vec;
	if ((vec = malloc(sizeof(vector))) == NULL) {
        perror("malloc failed.");
        free(vec);
        exit(3);
    }
	
	// and buffer within vector
	if ((vec->buff = malloc(init_capacity * sizeof(valType))) == NULL) {
		perror("malloc failed.");
        free(vec);
        exit(3);
	}

	// and initialize the buffer if init_vals provided
	if (init_vals != NULL) {
		memcpy(vec->buff, init_vals, init_size * sizeof(valType));
		vec->size = init_size;
	}
	else {
		vec->size = 0;
	}

	// finally, set the vector's size
	vec->capacity = init_capacity;

	return vec;
}

// adds an element to the vector, enlarging it if necessary
vector* append(vector *vec, valType val) {
	if (vec->size < vec->capacity) {
		// just add value to buffer
		vec->buff[vec->size] = val;
		vec->size++;
		return vec;
	}
	else {
		printf("expanding\n");
		// create new double sized vec, copy old to new, and then add new value
		vector *new_vec;
		new_vec = init_vec(2 * vec->capacity, vec->buff, vec->size);
		append(new_vec, val);

		return new_vec; 
	}
}
