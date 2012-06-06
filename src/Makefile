CC      = gcc
CFLAGS  = -Wall -pedantic -std=c99 -O0 -g

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate test_token test_sym

assemble: assemble.o
	$(CC) $(CFLAGS) -o $@ $^

emulate: emulate.o instructions.o utils.o
	$(CC) $(CFLAGS) -o $@ $^
test_sym: symbol_table_tests.o symbol_table.o
	$(CC) $(CFLAGS) -o $@ $^
test_token: test_tokeniser.o tokeniser.o
	$(CC) $(CFLAGS) -o $@ $^
test_pass: test_pass2.o symbol_table.o tokeniser.o pass1.o pass2.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f assemble
	rm -f emulate
	rm -f test_sym
	rm -f test_token
	rm -f test_pass
