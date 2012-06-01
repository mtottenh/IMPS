#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
/* THINGS TO CONSIDER:
 * 1. Hashtable implementation of symbol table
 * 2. Binary tree implementation?
 */

/* Types for symbol table*/
typedef key char*;
typedef value uint16_t;

/* Linekd list implementation of symbol table */
typedef struct symbol_table {
	key k;
	value v; 
	symbol_table *next;
};

/* Creates a new symbol table on the head returning a pointer*/
symbol_table* symbol_table_new(void);

/* Deletes a symbol table */
void symbol_table_free(symbol_table*);

/* Returns the size of the symbol_table pointed to by symbol_table* */
size_t symbol_table_getLength(symbol_table*);

/* Adds the (key,value) pair to the symbol table poitned to by symbol_table* */
int symbol_table_put(symbol_table*,key,value);

/* Removes all (key,_) pairs from the symbol table 
 * pointed to by symbol_table* 
 */
int symbol_table_remove(symbol_table*,key);

/* Checks to see if a (key,_) pair is in the symbol table 
 * pointed to by symbol_table* 
 */
int symbol_table_contains(symbol_table*,key);

/* Returns the first (key,value) pair in the symbol table
 * pointed to by symbol_table* 
 */
value symbol_table_get(symbol_table*,key);

/*symbol_table_map();*/

#endif
