#include "tokeniser.h"

void tokeniser_init(FILE* source, Tokeniser** tokeniser) {
	*tokeniser = malloc(sizeof(Tokeniser));
	Tokeniser* t = *tokeniser;
	t->file = source;
}

int get_tokenised_line(Tokeniser* tokeniser) {
	char buffer[100];
	if((fgets(buffer, 100, tokeniser->file)) == NULL)
	{
		printf("Error: EOF reached\n");
		return 1;
	}
	if (buffer[0] == '\n') return get_tokenised_line(tokeniser);
	tokeniser->line.num_operands = 0;
	char* buffer_ptr = buffer;
	char *token = strtok(buffer_ptr, " ");
	for (int i = 0; (token  != NULL); i++) {
		if (*token == '-'){
			break;	
		}
		tokens[i] = token;
		if (buffer_ptr != NULL) {
			buffer_ptr = NULL;
		}
		tokeniser->line.num_operands++;
		token = strtok(NULL, " ");
	}
	//Now we have a token array, with or without label and an unknown
	//number of operands. Great.

	if (strchr(tokens[0], ':') != NULL) {
		//If the first item is a label, add the label field.
		//Decrement num operand by 2, the label and opcode.
		tokeniser->line.label = tokens[0];
		tokeniser->line.opcode = tokens[1];
		tokeniser->line.operand1 = tokens[2];
		tokeniser->line.operand2 = tokens[3];
		tokeniser->line.operand3 = tokens[4];
		tokeniser->line.num_operands = tokeniser->line.num_operands - 2;
		}
	else {
		tokeniser->line.label = NULL;
		tokeniser->line.opcode = tokens[0];
		tokeniser->line.operand1 = tokens[1];
		tokeniser->line.operand2 = tokens[2];
		tokeniser->line.operand3 = tokens[3];
		tokeniser->line.num_operands--;
	}
	
	return 0;
	
}

void free_tokeniser(Tokeniser* tokeniser) {
	free(tokeniser);
}
