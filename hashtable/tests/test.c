#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash_table.h"
#include "data.h"

// This code is designed to test the correctness of your implementation. 
// You do not need to significantly change it. 
// Compile and run it in the command line by typing: 
// make test; ./test

int main(void) {
	int num_tests = 20;
	size_t key_len = 10;
	size_t val_len = 100;

	hashtable* ht = NULL;
	init(&ht);

	// create test keys and values
	keyType** keys;
	valType** vals;
	if ((keys = malloc(num_tests * sizeof(keyType*))) == NULL) {
		perror("malloc failed.");
		exit(3);
	}
	if ((vals = malloc(num_tests * sizeof(valType*))) == NULL) {
		perror("malloc failed.");
		exit(3);
	}

	for (int i = 0; i < num_tests; i++) {
		keyType* key;
		valType* val;
		if ((key = malloc((key_len + 1) * sizeof(keyType))) == NULL) {
			perror("malloc failed.");
			exit(3);
		}
		if ((val = malloc((val_len + 1) * sizeof(valType))) == NULL) {
			perror("malloc failed.");
			exit(3);
		}
		rand_string(key, key_len);
		rand_string(val, val_len);

		// size_t c;
		// printf("key: ");
		// for (c = 0; c < key_len; c++)
  //     		printf("%c", key[(int) c]);
  //     		printf("\n");
  //     	printf("val: ");
  //     	for (c = 0; c < val_len; c++)
  //     		printf("%c", val[(int) c]);
  //     		printf("\n");
	}


	printf("Testing putting and getting from the hash table.\n");
	printf("Inserting %d key-value pairs.\n", num_tests);
	for (int i = 0; i < num_tests; i += 1) {
		put(ht, keys[i], values[i]);
		put(ht, keys[i], values[i]);
		put(ht, keys[i], values[i]);
		put(ht, keys[i], values[i]);

		printf("\t(%d -> %d) \n", keys[i], values[i]);
	}

	int num_values = 1;
	int *results = NULL; // malloc(sizeof(valType) * num_values);
	int **results_ptr = &results;

	for (int i = 0; i < num_tests; i += 1) {
		int index = rand() % num_tests;
		keyType target_key = keys[index];
		get(ht, target_key, results_ptr, num_values);
		if (results[0] != values[index]) {
			printf("Test failed with key %d. Got value %d. Expected value %d.\n", target_key, results[0], values[index]);
			return 1;
		} 
	}

	printf("Passed tests for putting and getting.\n");
	printf("Now testing erasing.\n");

	for (int i = 0; i < num_tests; i += 1) {
		keyType target_key = keys[i];
		erase(ht, target_key);
		int num_matches = get(ht, target_key, results_ptr, num_values);
		if (num_matches != 0) {
			printf("Test failed with key %d. Expected it to be erased, but got %d matches.\n", target_key, num_matches);
			return 1;
		} 
	}
	del(ht);
	printf("Passed tests for erasing.\n");
	printf("All tests have been successfully passed.\n");
	return 0;
}
