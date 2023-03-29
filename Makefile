CC      = clang
CFLAGS  = -Wall -Wextra -Werror -Wpedantic -gdwarf-4
LFLAGS  = -lm

.PHONY: all clean format

all: encode decode

encode: encode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o encode encode.o node.o pq.o code.o io.o stack.o huffman.o -lm
decode: decode.o node.o pq.o code.o io.o stack.o huffman.o
	$(CC) -o decode decode.o node.o pq.o code.o io.o stack.o huffman.o -lm

node.o: node.c
	$(CC) $(CFLAGS) -c node.c
pq.o: pq.c
	$(CC) $(CFLAGS) -c pq.c
code.o: code.c
	$(CC) $(CFLAGS) -c code.c
io.o: io.c
	$(CC) $(CFLAGS) -c io.c
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c
huffman.o: huffman.c
	$(CC) $(CFLAGS) -c huffman.c
encode.o: encode.c
	$(CC) $(CFLAGS) -c encode.c
decode.o: decode.c
	$(CC) $(CFLAGS) -c decode.c

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -rf encode decode encode.o decode.o node.o pq.o code.o io.o stack.o huffman.o

spotless:
	rm -rf encode.o decode.o node.o pq.o code.o io.o stack.o huffman.o

scan-build: clean
	scan-build make


