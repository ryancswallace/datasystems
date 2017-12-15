#ifndef HASH_TABLE_GUARD
#define HASH_TABLE_GUARD

#include <stdlib.h>

#include "data.h"

// a single node of linked-list of arrays of a hashtable
typedef struct node {
	// all of the following are 8 bytes on x86-64, so alignment isn't a concern
	// totals 8 * 5 = 40 bytes on x86-64
	struct node *next_node; 
	struct node *prev_node;

	size_t size; 

	keyType *keys;
	valType *vals;
} node;

// defines a hash table of linked-list values
typedef struct hashtable {
	// totals 8 * 4 = 32 bytes on x86-64
	size_t num_elements;
	size_t num_buckets;
	size_t node_capacity;

	node **data;
} hashtable;

void init(hashtable** ht);
void del(hashtable* ht);
void put(hashtable* ht, keyType key, valType value);
int get(hashtable* ht, keyType key, valType **values, int num_values);
void erase(hashtable* ht, keyType key);

#endif
