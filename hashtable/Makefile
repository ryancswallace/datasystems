CC = gcc -std=c99
CFLAGS = -ggdb3 -g -W -Wall -Wextra -Werror -O3 -fsanitize=address
LDFLAGS =
LIBS =

default: main test benchmark vector

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: hash_table.o vector.o main.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test: hash_table.o vector.o test.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

benchmark: hash_table.o vector.o benchmark.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

vector: hash_table.o vector.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f main test benchmark vector *.o
