#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "hash_table.h"

// This code is designed to test the correctness of your implementation. 
// You do not need to significantly change it. 
// Compile and run it in the command line by typing: 
// make test; ./test

// copies random characters into array pointed to by s
void gen_random_string(char *s, int len) {
    static char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789";
 
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
 
    s[len] = "\0";
}

int main(void) {
	hashtable* ht=NULL;
	init(&ht);

	int seed = 1;
	srand(seed);

	int num_tests = 20;
	key_len = 10;
	val_len = 100;
	keyType *keys = malloc(num_tests * sizeof(key *));
	valType *vals = malloc(num_tests * sizeof(val *));

	printf("Testing putting and getting from the hash table.\n");
	printf("Inserting %d key-value pairs.\n", num_tests);
	for (int i = 0; i < num_tests; i += 1) {
		keys[i] = malloc(key_len);
		values[i] = malloc(val_len);

		gen_random_string(keys[i], key_len);
		gen_random_string(values[i], val_len);

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
