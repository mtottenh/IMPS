#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include <stdint.h>
#include <string.h>
/* THINGS TO CONSIDER:
 * 1. Hashtable implementation of symbol table
 * 2. Binary tree implementation?
 * 3. doubble check behaviour of symbol_table_free()
 */
enum opcodes {
	HALT,
	ADD,
	ADDI,
	SUB,
	SUBI,
	MUL,
	MULI,
	LW,
	SW,
	BEQ,
	BNE,
	BLT,
	BGT,
	BLE,
	BGE,
	JMP,
	JR,
	JAL,
	OUT,
	FILL,
	SKIP,
	PUSH,
	POP,
	CALL,
	RET
};
/* Types for symbol table*/

typedef char* Key;
typedef uint16_t Value;
/* Linekd list implementation of symbol table */
typedef struct  Symbol_Table_Entry {
	Key key;
	Value value; 
	struct Symbol_Table_Entry *next;
} Symbol_Table_Entry;


typedef struct {
	Symbol_Table_Entry *head;
} Symbol_Table; 
/* 
 * Creates a new symbol table on the heap
 * containing opcode and directive mappings
 * returns a pointer to the table
 */
Symbol_Table* symbol_table_new(void);

/* Deletes a symbol table */
void symbol_table_free(Symbol_Table**);

/* 
 * Returns the number of (key,value) pairs 
 * in the Symbol_Table pointed to by Symbol_Table* 
 */
size_t symbol_table_getLength(Symbol_Table*);
Symbol_Table_Entry* symbol_table_getPrev(Symbol_Table*, Key);
/* Adds the (key,value) pair to the symbol table poitned to by Symbol_Table* */
int symbol_table_put(Symbol_Table*,Key,Value);

/* 
 * Removes all (key,_) pairs from the symbol table 
 * pointed to by Symbol_Table* 
 */
int symbol_table_remove(Symbol_Table*,Key);

/*
 * Checks to see if a (key,_) pair is in the symbol table 
 * pointed to by Symbol_Table*, returns 0 on key contained
 * and 1 on key not found
 */
int symbol_table_contains(Symbol_Table*,Key);

/*
 * Returns  a pointer to the first (key,value) pair
 * in the symbol table pointed to by Symbol_Table* 
 */
Symbol_Table_Entry* symbol_table_get(Symbol_Table*,Key);

/*symbol_table_map();*/

/* Prints the given symbol table to the screen */
void symbol_table_print(Symbol_Table*);

#endif
