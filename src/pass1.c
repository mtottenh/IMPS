#include "pass1.h"

/* This being in a seperate C file is somewhat unnecessary. Put in somewhere else (assemble.c?) later. */

/* Performs the first pass of generating a binary assembler file. */
void pass1(FILE* file, Symbol_Table* table) {
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
		 * table. 
		 */
		if (label != NULL) {
			Symbol_Table_put(table, label, address);	
		}

		address += 4;
	}
		

	/* Dealllocate memory from tokeniser. */
	free_tokeniser(tokeniser);
}
