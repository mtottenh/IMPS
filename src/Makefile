CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99 -O0 -g

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate test

assemble: assemble.o
	$(CC) $(CFLAGS) -o $@ $^

emulate: emulate.o instructions.o utils.o
	$(CC) $(CFLAGS) -o $@ $^
test: symbol_table_tests.o symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^



clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
	rm -f test
	rm -f test_tokeniser
