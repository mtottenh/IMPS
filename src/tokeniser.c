#include "tokeniser.h"

Tokeniser* tokeniser_new(FILE* source) {
	
	Tokeniser *tokeniser = malloc(sizeof(Tokeniser));
	tokeniser->file = source;
	tokeniser->line.num_operands = 0;
	return tokeniser;
}

int get_tokenised_line(Tokeniser* tokeniser) {
	char buffer[20];
	if((fgets(buffer, 20, tokeniser->file)) == NULL)
	{
		printf("Error: EOF reached\n");
		return 1;
	} else {
		char* state;
		char* token;
		char* tokens[5];
		char* buffer_ptr = buffer;
		for (int i = 0; (token = strtok_r(buffer_ptr, " ", &state)) != NULL; i++) {
			tokens[i] = token;
			if (buffer_ptr != NULL) {
				buffer_ptr = NULL;
			}
			tokeniser->line.num_operands++;
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
	
}

void free_tokeniser(Tokeniser* tokeniser) {
	free(tokeniser);
}
