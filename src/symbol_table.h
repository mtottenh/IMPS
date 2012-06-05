#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
#include <stdint.h>
#include <string.h>
/* THINGS TO CONSIDER:
 * 1. Hashtable implementation of symbol table
 * 2. Binary tree implementation?
 */

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
 * returns a pointer
 */
Symbol_Table* Symbol_Table_new(void);

/* Deletes a symbol table */
void Symbol_Table_free(Symbol_Table**);

/* 
 * Returns the number of (key,value) pairs 
 * in the Symbol_Table pointed to by Symbol_Table* 
 */
size_t Symbol_Table_getLength(Symbol_Table*);

/* Adds the (key,value) pair to the symbol table poitned to by Symbol_Table* */
int Symbol_Table_put(Symbol_Table*,Key,Value);

/* 
 * Removes all (key,_) pairs from the symbol table 
 * pointed to by Symbol_Table* 
 */
int Symbol_Table_remove(Symbol_Table*,Key);

/*
 * Checks to see if a (key,_) pair is in the symbol table 
 * pointed to by Symbol_Table* 
 */
int Symbol_Table_contains(Symbol_Table*,Key);

/*
 * Returns the first (key,value) pair in the symbol table
 * pointed to by Symbol_Table* 
 */
Symbol_Table_Entry* Symbol_Table_get(Symbol_Table*,Key);

/*Symbol_Table_map();*/
/* Prints the given symbol table to the screen*/
void Symbol_Table_print(Symbol_Table*);

#endif
