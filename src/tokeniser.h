#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Tokeniser_Line {
	char* label;
	char* opcode;
	char* operand1;
	char* operand2;
	char* operand3;
	int num_operands;
} Tokeniser_Line;

typedef struct Tokeniser {
	FILE *file;
	Tokeniser_Line line;
} Tokeniser;

Tokeniser* tokeniser_new(FILE*);
int get_tokenised_line(Tokeniser*);
void free_tokeniser(Tokeniser*);
