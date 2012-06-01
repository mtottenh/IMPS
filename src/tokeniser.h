#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	char* label;
	char* opcode;
	char* operand1;
	char* operand2;
	char* operand3;
	int num_operands;
} Tokenised_Line;

typedef struct {
	FILE *file;
	Tokenised_Line line;
} Tokeniser;

void tokeniser_init(FILE*, Tokeniser*);
int get_tokenised_line(Tokeniser*);
void free_tokeniser(Tokeniser*);
