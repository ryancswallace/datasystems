#include <stdlib.h>
#include <string.h>

// inserts a string of len random characters in str
void rand_string(char* str, size_t len) {
	const char* char_choices = "abcdefghijklmnopqrstuvwxyz0123456789";
	char rand_buff[len + 1];
	for (size_t i = 0; i < len; i++) {

		rand_buff[i] = char_choices[(rand() % strlen(char_choices))];
	}
	memcpy(str, rand_buff, len);
	rand_buff[len] = '\0';
}

