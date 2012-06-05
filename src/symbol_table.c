#include "symbol_table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/* Allocates enough space for a new Symbol_Table on the heap */
Symbol_Table* Symbol_Table_new(void) {
	Symbol_Table* s = malloc(sizeof(Symbol_Table));
	/* Add opcodes and Directives to the start of the symbol table */	
	Symbol_Table_put(s,"halt",0); 
	Symbol_Table_put(s,"add",1);
	Symbol_Table_put(s,"addi",2);
	Symbol_Table_put(s,"sub",3);
	Symbol_Table_put(s,"subi",4);
	Symbol_Table_put(s,"mul",5);
	Symbol_Table_put(s,"muli",6);
	Symbol_Table_put(s,"lw",7);
	Symbol_Table_put(s,"sw",8);
	Symbol_Table_put(s,"beq",9);
	Symbol_Table_put(s,"bne",10);
	Symbol_Table_put(s,"blt",11);
	Symbol_Table_put(s,"bgt",12);
	Symbol_Table_put(s,"ble",13);
	Symbol_Table_put(s,"bge",14);
	Symbol_Table_put(s,"jmp",15);
	Symbol_Table_put(s,"jr",16);
	Symbol_Table_put(s,"jal",17);
	Symbol_Table_put(s,"out",18);
	Symbol_Table_put(s,".fill",19);
	Symbol_Table_put(s,".skip",20);
	
	return s;
}

/* 
 * Deletes a symbol table by repeatedly looping to the end of 
 * the symbol table and droping the last element
 * until no elements are left
 */
void Symbol_Table_free_aux(Symbol_Table_Entry *s) {
	if(s != NULL) {
		Symbol_Table_free_aux(s->next);
		free(s->key);
		s->key = NULL;
		free(s);
		s = NULL;
	
	}
}
void Symbol_Table_free(Symbol_Table **s) {
	Symbol_Table_free_aux((*s)->head);
	free(*s);
	*s = NULL;
}
/* NB Assumes that every non NULL entry is a real entry in the table! */
size_t Symbol_Table_getLength(Symbol_Table *table) {
	if (table == NULL) {
		return -1;
	}
	size_t counter = 0;
	for(Symbol_Table_Entry *i = table->head; i->next != NULL; i=i->next) {
		counter++;
	}	
	return counter;
}
/* **TODO** Add error trapping code*/
int Symbol_Table_put(Symbol_Table* table, Key key, Value value) {
	/* PRE: table is a valid symbol table ie not NULL*/
	Symbol_Table_Entry *i = table->head;
	if (table->head == NULL) {
		table->head = malloc(sizeof(Symbol_Table_Entry));
		table->head->key = malloc(sizeof(key));
		table->head->key = strncpy(table->head->key,key,sizeof(key));
		table->head->value = value;
		table->head->next = NULL;
		return 0;
	}
	while(  i->next != NULL) {
		 i=i->next; 	
	}
	/* MID : i now points to the last entry in the table */
	/* create a new entyr on the end of the list */
	i->next = malloc(sizeof(Symbol_Table_Entry));
	i = i->next;
	/* Set it to hold (key,value) and point to NULL */
	i->key = malloc(sizeof(key));
	strncpy(i->key,key,sizeof(key));
	i->value = value;
	i->next = NULL;
	/* Return 0 on no error */
	return 0;
}
Symbol_Table_Entry* Symbol_Table_getPrev(Symbol_Table* table, Key key) {
	Symbol_Table_Entry* s = table->head;
	/*If the head of the table is what is being searched for we have no previous element. */
	if (strcmp(s->key,key) == 0) {
		return NULL;
	}
	while ((strcmp(s->next->key,key) != 0) && s != NULL) {
		s = s->next;
	}
	return s;
}
int Symbol_Table_remove(Symbol_Table* table, Key key) {
	Symbol_Table_Entry *current = Symbol_Table_get(table,key);
	Symbol_Table_Entry *prev = Symbol_Table_getPrev(table,key);
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

int Symbol_Table_contains(Symbol_Table* table, Key key) {
	Symbol_Table_Entry *s = table->head;
	while ((strcmp(s->key,key) != 0) && s != NULL) {
		s = s->next;
	}
	return s == NULL ? 1 : 0;
}

/*
 * Returns a the given (key,value) pair corresponding to the given key 
 * Note that if key is not in the table, s will become equal to NULL
 * the last entry in the list. 
 */
Symbol_Table_Entry* Symbol_Table_get(Symbol_Table* table, Key key) {
	Symbol_Table_Entry *s = table->head;
	while((strcmp(s->key,key) !=0) && s != NULL){
		s = s->next;
	}
	return s;
}


void Symbol_Table_print(Symbol_Table* table) {
	if (table == NULL) {
		printf("Erorr: Not a valid symbol table, please call Symbol_Table_new()\n");
		return;
	}

	Symbol_Table_Entry *s = table->head;
	for (;s != NULL; s = s->next) {

		printf("Key: %s\t Value: %d\t", s->key, s->value);
	}
}
