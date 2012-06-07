#include "pass1.h"

/* This being in a seperate C file is somewhat unnecessary. Put in somewhere else (assemble.c?) later. */

/* Performs the first pass of generating a binary assembler file. */
void pass1(FILE* file, Symbol_Table* table) {
	printf("****PASS 1****\n\n");
	/* Create new tokeniser pointer and initialise it. */
	Tokeniser* tokeniser;
 	tokeniser_init(file,&tokeniser);

	/* Loop through the file until EOF or an error occurs. */
	uint16_t address = 0;
	char* label;
	Tokeniser_Line line;
		
	while(get_tokenised_line(tokeniser) == 0) {
		line = tokeniser->line;
		label = line.label;
		/* 
		 * If we have a label, record it and the address in the symbol
		 * table. Remove the colon! 
		 */
		if (label != NULL) {
			int last_elem = strlen(label) - 1;
			label[last_elem] = '\0';
			printf("Label: %s\tAddress: %u\n", label, address);
			Symbol_Table_put(table, label, address);	
		}

		/*
		 * Check if we have .skip (opcode 20) which changes address
		 * offsets.
		 */
		if (Symbol_Table_get(table, line.opcode)->value == 20) {
			address += atoi(line.operand1)*4;
		} else {
 			address += 4;
		}
	}
		

	/* Dealllocate memory from tokeniser. */
	free_tokeniser(tokeniser);
}
