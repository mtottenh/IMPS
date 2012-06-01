#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_
/*Temp typedef to make compling work */
typedef symbol_table int;

/* Creates a new symbol table on the head returning a pointer*/
symbol_table* symbol_table_new(void);
/* Deletes a symbol table */
void symbol_table_free(symbol_table*);
/* returns the size of the symbol_table pointed to by symbol_table* */
size_t symbol_table_getLength(symbol_table*);
/* Adds the (key,value) pair to the symbol table poitned to by symbol_table* */
int symbol_table_put(symbol_table*,key,value);
/* removes all (key,_) pairs from the symbol table 
 * pointed to by symbol_table* 
 */
int symbol_table_remove(symbol_table*,key);
/* checks to see if a (key,_) pair is in the symbol table 
 * pointed to by symbol_table* 
 */
int symbol_table_contains(symbol_table*,key);
/* returns the first (key,value) pair in the symbol table
 * pointed to by symbol_table* 
 */
value symbol_table_get(symbol_table*,key);
/*symbol_table_map();*/

#endif
