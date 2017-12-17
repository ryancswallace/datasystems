CC = gcc -std=c99
CFLAGS = -Ivector -Ilib -Itests -Isrc -ggdb3 -g -W -Wall -Wextra -Werror -O3 -fsanitize=address
LDFLAGS =
LIBS =

default: src/main tests/test tests/benchmark lib/vector

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: src/hash_table.o lib/vector.o src/main.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test: src/hash_table.o lib/vector.o tests/test.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

benchmark: src/hash_table.o lib/vector.o tests/benchmark.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

vector: src/hash_table.o lib/vector.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f main test benchmark vector *.o
