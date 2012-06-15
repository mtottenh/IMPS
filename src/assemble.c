#include "pass1.h"
#include "pass2.h"

int main(int argc, char** argv) {
	/* Get input and output file pointers from command line arguments. */
	FILE* input = fopen(argv[1], "rt");
	if (input == NULL) {
		perror("Could not open input file. Terminating.");
		exit(EXIT_FAILURE);
	}

	FILE* output = fopen(argv[2], "wb+");
	if (output == NULL) {
		perror("Could not open output file. Terminating.");
		exit(EXIT_FAILURE);
	}

	/* Create a new symbol table for the two passes. */
	Symbol_Table* table = symbol_table_new();

	/* Perform pass 1, then reopen the input file and perform pass 2. */
	pass1(input, table);
	fclose(input);
	
	input = fopen(argv[1], "rt");
	pass2(input, output, table);

	/* Close both file streams and exit. */
	fclose(input);
	fclose(output);

	/* Free the symbol table's memory. */
	symbol_table_free(&table);

	return EXIT_SUCCESS;
}
