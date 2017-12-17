/**
    CS 165/265 Project 0
    hash_table.c
    Purpose: Implements an efficient associative array via a hash table.

    @author Ryan Wallace
*/

#include <stdio.h>
#include <assert.h>

#include "hash_table.h"
#include "vector.h"

#define NUM_BUCKETS 2501383

// initialize the components of the hashtable
void init(hashtable **ht) {
    // data is a pointer to an array of pointers to linked-lists of nodes
    hashtable *tbl;
    if ((tbl = malloc(sizeof(hashtable))) == NULL) {
        perror("malloc failed.");
        free(tbl);
        exit(3);
    }
    
    tbl->data = (node **) calloc(NUM_BUCKETS, sizeof(node *));

    tbl->num_elements = 0;
    tbl->num_buckets = NUM_BUCKETS;
    tbl->node_capacity = (128 - 2 * sizeof(node *) - sizeof(size_t)) / (sizeof
        (keyType) + sizeof(valType)); // try to fit node and data in cash line
    *ht = tbl;
}

// initializes a single node of a linked-list
void init_node(node **nd, size_t node_capacity) {
    if ((*nd = malloc(sizeof(node))) == NULL) {
        perror("malloc failed.");
        free(*nd);
        exit(3);
    }

    (*nd)->next_node = NULL;
    (*nd)->prev_node = NULL;
    (*nd)->size = 0;
    if (((*nd)->keys = malloc(node_capacity * sizeof(keyType))) == NULL) {
        perror("malloc failed.");
        free((*nd)->keys);
        exit(3);
    }
    if (((*nd)->vals = malloc(node_capacity * sizeof(valType))) == NULL) {
        perror("malloc failed.");
        free((*nd)->vals);
        exit(3);
    }
}

// frees all dynamically allocated memory associated with a hashtable
void del(hashtable* ht) {
    for (size_t bucket = 0; bucket < ht->num_buckets; bucket++) {
        node *next_node = ht->data[bucket];
        node *curr_node;
        while ((curr_node = next_node) != NULL) {
            // free each array and then the containing node
            free(curr_node->keys);
            free(curr_node->vals);
            next_node = curr_node->next_node;
            free(curr_node);
        }
    }
    free(ht);
}

// hashes the value of key
static inline
int hash(keyType key) {
    return key % NUM_BUCKETS;
}

// insert a key-value pair into the hash table. Allows for mutliple identical
// keys
void put(hashtable* ht, keyType key, valType value) {
    int hashed_key = hash(key);

    // check whether bucket is empty; if so, initialize node there
    if (ht->data[hashed_key] == NULL) {
        node *new_node = NULL;
        init_node(&new_node, ht->node_capacity);

        ht->data[hashed_key] = new_node;
    }
    assert(ht->data[hashed_key] != NULL);
    
    // add to the beginning of the list to avoid traversal
    node *first_node = ht->data[hashed_key];

    // if first node is full, add new node
    if (first_node->size >= ht->node_capacity) {
        node *new_node;
        init_node(&new_node, ht->node_capacity);

        first_node->prev_node = new_node;
        new_node->next_node = first_node;
        first_node = new_node;
    }
    assert(first_node->size < ht->node_capacity);

    // add new data
    *(first_node->keys + first_node->size) = key;
    *(first_node->vals + first_node->size) = value;
    ++first_node->size;
}

/* 
    get entries in ht matching key. If values points to a NULL pointer (i.e., the caller has not allocated space for the return values), get() allocates sufficient memory to return all matching values. In this case, it is the RESPONSIBILITY OF THE CALLER to free this memory. Otherwise, if *values is not NULL, the size of the values array must be specified by the parameter
    num_values. If there are more matching entries than num_values, additional values are not stored in the values array to avoid a buffer overflow. The function returns the number of matching entries. If the return value is greater than num_values, the caller can invoke this function again (with a larger buffer) to get values that it missed during the first call. 
*/
int get(hashtable* ht, keyType key, valType **values, int num_values) {
    int hashed_key = hash(key);
    int num_matches = 0;   

    node *curr_node = ht->data[hashed_key];
    if (curr_node == NULL) {
        // nothing to look for
        return 0;
    }
    
    if (*values != NULL) {
        // don't need to allocate array ourselves
        // loop through each node and array, pulling out matching values
        do {
            for (size_t i = 0; i < curr_node->size; i++) {
                if (curr_node->keys[i] == key) {
                    num_matches += 1;
                    if (num_matches <= num_values) {
                        // only add value if space remains
                        *values[num_matches - 1] = curr_node->vals[i];
                    }
                }
            }
        } while((curr_node = curr_node->next_node) != NULL);
    }
    else {
        // need to allocate ourselves
        vector *values_vec = init_vec((size_t) num_values, NULL, 0);
        
        // loop like above, but add all values
        do {
            for (size_t i = 0; i < curr_node->size; i++) {
                if (curr_node->keys[i] == key) {
                    num_matches += 1;
                    values_vec = append(values_vec, curr_node->vals[i]);
                }
            }
        } while((curr_node = curr_node->next_node) != NULL);

        // point values to data buffer of vector we created
        *values = values_vec->buff;
    }

    return num_matches;
}

// erase a key-value pair from the hash table
void erase(hashtable* ht, keyType key) {
    int hashed_key = hash(key);

    node *curr_node = ht->data[hashed_key];
    node *next_node;
    if (curr_node == NULL) {
        // hashed bucket is empty, nothing to do
        return;
    }
    else {
        while(1) {
            // check if key exists in bucket
            int size = (int) curr_node->size;
            for (int i = 0; i < size; i++) {
                // set next node before potentially removing current node
                next_node = curr_node->next_node;

                if (curr_node->keys[i] == key) {
                    // remove pair and reshuffle array to preserve contiguity
                    if (curr_node->size > 1) {
                        // replace element to be removed with last element
                        curr_node->keys[i] = curr_node->keys[curr_node->size - 1];
                        curr_node->vals[i] = curr_node->vals[curr_node->size - 1];
                        i--; // need to recheck i as it's now the last element
                    }
                    curr_node->size--;

                    if (curr_node->size == 0) {
                        // free key and value arrays
                        free(curr_node->keys);
                        free(curr_node->vals);

                        // rearrange linked-list and free node
                        if (curr_node->next_node != NULL) {
                            if (curr_node->prev_node != NULL) {
                                curr_node->prev_node->next_node =
                                curr_node->next_node;
                                curr_node->next_node->prev_node =
                                curr_node->prev_node;
                            }
                            else {
                                curr_node->next_node->prev_node = NULL;
                                ht->data[hashed_key] = curr_node->next_node;
                            }
                        }
                        else {
                            // removing terminal node
                            if (curr_node->prev_node != NULL) {
                                curr_node->prev_node->next_node = NULL;
                            }
                            else {
                                // removing last node in bucket
                                ht->data[hashed_key] = NULL;
                            }
                        }

                        free(curr_node);
                        break;
                    }
                }
            }

            if (next_node == NULL) {
                // key not found; no more to check
                break;
            }
            else {
                // check next node
                curr_node = next_node;
            }
        }
    }
}
