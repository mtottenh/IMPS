CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

emulate: emulate.o instructions.o utils.o
	$(CC) $(CFLAGS) -o $@ $^
assemble: assemble.o symbol_table.o tokeniser.o pass1.o pass2.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
