#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Allocates enough space for a new Symbol_Table on the heap */
Symbol_Table* symbol_table_new(void) {
	Symbol_Table* s = malloc(sizeof(Symbol_Table));
	/* Add opcodes and Directives to the start of the symbol table */	
	symbol_table_put(s, "halt", HALT); 
	symbol_table_put(s, "add", ADD);
	symbol_table_put(s, "addi", ADDI);
	symbol_table_put(s, "sub", SUB);
	symbol_table_put(s, "subi", SUBI);
	symbol_table_put(s, "mul", MUL);
	symbol_table_put(s, "muli", MULI);
	symbol_table_put(s, "lw", LW);
	symbol_table_put(s, "sw", SW);
	symbol_table_put(s, "beq", BEQ);
	symbol_table_put(s, "bne", BNE);
	symbol_table_put(s, "blt", BLT);
	symbol_table_put(s, "bgt", BGT);
	symbol_table_put(s, "ble", BLE);
	symbol_table_put(s, "bge", BGE);
	symbol_table_put(s, "jmp", JMP);
	symbol_table_put(s, "jr", JR);
	symbol_table_put(s, "jal", JAL);
	symbol_table_put(s, "out", OUT);
	symbol_table_put(s, ".fill", FILL);
	symbol_table_put(s, ".skip", SKIP);
	symbol_table_put(s, "push", PUSH);
	symbol_table_put(s, "pop", POP);
	symbol_table_put(s, "call", CALL);
	symbol_table_put(s, "ret", RET);
	symbol_table_put(s, "mov", MOV);
	symbol_table_put(s, "inc", INC);
	return s;
}

/* 
 * Deletes a symbol table by repeatedly looping to the end of 
 * the symbol table and droping the last element
 * until no elements are left
 */
static void symbol_table_free_aux(Symbol_Table_Entry **entry_ptr) {
	if(*entry_ptr != NULL) {
		symbol_table_free_aux(&((*entry_ptr)->next));
		free((*entry_ptr)->key);
		(*entry_ptr)->key = NULL;
		free(*entry_ptr);
		*entry_ptr = NULL;
	}
}

void symbol_table_free(Symbol_Table **entry_ptr) {
	symbol_table_free_aux(&((*entry_ptr)->head));
	free(*entry_ptr);
	*entry_ptr = NULL;
}

/* NB Assumes that every non NULL entry is a real entry in the table! */
size_t symbol_table_getLength(Symbol_Table *table) {
	if (table == NULL) {
		return -1;
	}
	size_t counter = 0;
	for(Symbol_Table_Entry *i = table->head; i->next != NULL; i=i->next) {
		counter++;
	}	
	return counter;
}

int symbol_table_put(Symbol_Table* table, Key key, Value value) {
/* PRE: table is a valid symbol table ie not NULL*/
/* POST: Returns 0 on successfull addition of elem to table and 1 on error */
	Symbol_Table_Entry **entry_ptr = &(table->head);
	/* Adding to an empty table */
	if ((*entry_ptr) == NULL ) {
		(*entry_ptr) = malloc(sizeof(Symbol_Table_Entry));
		if ((*entry_ptr) == NULL) {
			perror("Error malloc'ing in emptry table");
			return 1;
		}
	} else {
		while((*entry_ptr)->next != NULL) {
			 entry_ptr = &((*entry_ptr)->next); 	
		}
		/* MID1 : i now points to the last entry in the table*/
		/* create a new entry on the end of the list */
		(*entry_ptr)->next = malloc(sizeof(Symbol_Table_Entry));
		if ((*entry_ptr)->next == NULL) {
			perror("malloc'ing space for i->next");
			return 1;
		} 
		entry_ptr = &((*entry_ptr)->next);
	}
	/* MID2 : i now points to a new entry at the  end of table */
	/* Set it to hold (key,value) and point to NULL as the next elem */
	(*entry_ptr)->key = malloc(strlen(key));
	if ((*entry_ptr)->key == NULL) {
		perror("malloc'ing space for key");
		return 1;
	}

	strncpy((*entry_ptr)->key, key, strlen(key));
	(*entry_ptr)->value = value;
	(*entry_ptr)->next = NULL;
	/* Return 0 on no error */

	return 0;
}
/* 
 * This is declared as static as it is only a helper fucntion 
 * and is not used outside the file.
 */
static Symbol_Table_Entry* symbol_table_getprev(Symbol_Table* table, Key key) {

	Symbol_Table_Entry* s = table->head;
	/*
	 * If the head of the table is what is being searched for 
	 * we have no previous element. 
 	 */
	if (strcmp(s->key,key) == 0) 
		return NULL;

	while ((strcmp(s->next->key,key) != 0) && s != NULL) {
		s = s->next;
	}

	return s;
}

int symbol_table_remove(Symbol_Table* table, Key key) {

	Symbol_Table_Entry *current = symbol_table_get(table,key);
	Symbol_Table_Entry *prev = symbol_table_getprev(table,key);

	if (prev == NULL) {
		table->head = current->next;
	} else {
		prev->next = current->next;
	}

	free(current->key);
	free(current);
	current = NULL;
	return 0;
}

int symbol_table_contains(Symbol_Table* table, Key key) {

	Symbol_Table_Entry *s = table->head;

	while ((strcmp(s->key, key) != 0) && s != NULL) {
		s = s->next;
	}

	return s == NULL ? 1 : 0;
}

/*
 * Returns the first (key,value) pair corresponding to the given key 
 * Note that if key is not in the table, s will become equal to NULL
 * the last entry in the list. 
 */
Symbol_Table_Entry* symbol_table_get(Symbol_Table* table, Key key) {

	Symbol_Table_Entry *entry_ptr = table->head;
	
	while((entry_ptr != NULL) && (strcmp(entry_ptr->key, key) !=0)) {
		entry_ptr = entry_ptr->next;
	}

	return entry_ptr;
}


void symbol_table_print(Symbol_Table* table) {
	if (table == NULL) {
		fprintf(stderr,"Erorr: Not a valid symbol table," 
				"please call symbol_table_new()\n");
		return;
	}

	Symbol_Table_Entry *entry_ptr = table->head;

	for ( ; entry_ptr != NULL; entry_ptr = entry_ptr->next) {
		printf("Key: %s\t Value: %d\n", 
			entry_ptr->key, entry_ptr->value);
	}
}
