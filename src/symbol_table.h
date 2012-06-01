#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
/* THINGS TO CONSIDER:
 * 1. Hashtable implementation of symbol table
 * 2. Binary tree implementation?
 */

/* Types for symbol table*/
typedef key char*;
typedef value uint16_t;

typedef struct Symbol_Table {
	Symbol_Table_Entry *head;
}
/* Linekd list implementation of symbol table */
typedef struct Symbol_Table_Entry {
	key k;
	value v; 
	Symbol_Table *next;
};

/* Creates a new symbol table on the head returning a pointer*/
Symbol_Table* Symbol_Table_new(void);

/* Deletes a symbol table */
void Symbol_Table_free(Symbol_Table*);

/* Returns the size of the Symbol_Table pointed to by Symbol_Table* */
size_t Symbol_Table_getLength(Symbol_Table*);

/* Adds the (key,value) pair to the symbol table poitned to by Symbol_Table* */
int Symbol_Table_put(Symbol_Table*,key,value);

/* 
 * Removes all (key,_) pairs from the symbol table 
 * pointed to by Symbol_Table* 
 */
int Symbol_Table_remove(Symbol_Table*,key);

/*
 * Checks to see if a (key,_) pair is in the symbol table 
 * pointed to by Symbol_Table* 
 */
int Symbol_Table_contains(Symbol_Table*,key);

/*
 * Returns the first (key,value) pair in the symbol table
 * pointed to by Symbol_Table* 
 */
value Symbol_Table_get(Symbol_Table*,key);

/*Symbol_Table_map();*/

#endif
