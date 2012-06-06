#include "pass1.h"

int main(int argc, char** argv) {
	FILE* input;
	input = fopen(argv[1], "rt");

	Symbol_Table* table = Symbol_Table_new();
	
	printf("***** BEFORE *****\n\n\n");
	Symbol_Table_print(table);

	pass1(input, table);
	printf("\n\n\n***** AFTER *****\n\n\n");
	Symbol_Table_print(table);	
}
