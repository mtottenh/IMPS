#include "pass1.h"

/*
 * Performs the first pass of generating a binary assembler file. In this stage, we identify labels, 
 * and assign them addresses.
 */
void pass1(FILE* file, Symbol_Table* table) {
	/* Create new tokeniser pointer and initialise it. */
	Tokeniser* tokeniser = tokeniser_new(file);

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
			symbol_table_put(table, label, address);	
		}

		/* Check if we have .skip, which changes address offsets. */
		if (symbol_table_get(table, line.opcode)->value == SKIP_OPCODE) {
			address += atoi(line.operand1) * 4;
		} else {
 			address += 4;
		}
	}
		

	/* Dealllocate memory from tokeniser. */
	free_tokeniser(tokeniser);
}
