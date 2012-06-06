#include "pass2.h"

int main(int argc, char** argv) {
	FILE* input = fopen(argv[1], "rt");
	FILE* output = fopen(argv[2], "wt");

	Symbol_Table* table = Symbol_Table_new();
	pass1(input, table);
	fclose(input);
	
	input = fopen(argv[1], "rt");
	pass2(input, output, table);

	fclose(input);
	fclose(output);

	return 0;
}
