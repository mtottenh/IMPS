#include "tokeniser.h"
//#define _POSIX_C_SOURCE 200809L /* or greater */

void tokeniser_init(FILE* source, Tokeniser** tokeniser) {
	*tokeniser = malloc(sizeof(Tokeniser));
	Tokeniser* t = *tokeniser;
	t->file = source;
}
/* allocates dest on the heap and coppies the value of src into dest*/
int copy_token(char **dest, const void *src) {
	if (src == NULL) {
		*dest = NULL; 
		return 1;
	}
	*dest = malloc(strlen(src));
	strncpy(*dest,src,strlen(src));
	return 0;	
}
int get_tokenised_line(Tokeniser* tokeniser) {
	char buffer[100];
	memset(buffer, 0, sizeof(buffer));

	if((fgets(buffer, 100, tokeniser->file)) == NULL)
	{
		printf("Error: EOF reached\n");
		return 1;
	}
	if (buffer[0] == '\n') 
		return get_tokenised_line(tokeniser);

	tokeniser->line.num_operands = 0;
	//char* state = NULL;
	char* tokens[5];
	memset(tokens, '\0', sizeof(tokens));
	char* buffer_ptr = buffer;
	char *token = strtok(buffer_ptr, " \n");
	for (int i = 0; (token  != NULL) && i < 4; i++) {
		tokens[i] = token;
		if (buffer_ptr != NULL) {
			buffer_ptr = NULL;
		}
		tokeniser->line.num_operands++;
		token = strtok(buffer_ptr, " \n");
	}
	//Now we have a token array, with or without label and an unknown
	//number of operands. Great.

	if (strchr(tokens[0], ':') != NULL) {
		//If the first item is a label, add the label field.
		//Decrement num operand by 2, the label and opcode.
		copy_token(&tokeniser->line.label,tokens[0]);
		copy_token(&tokeniser->line.opcode,tokens[1]);
		copy_token(&tokeniser->line.operand1,tokens[2]);
		copy_token(&tokeniser->line.operand2,tokens[3]);
		copy_token(&tokeniser->line.operand3,tokens[4]);

					
	} else {
		tokeniser->line.label = NULL;
		copy_token(&tokeniser->line.opcode,tokens[0]);
		copy_token(&tokeniser->line.operand1,tokens[1]);
		copy_token(&tokeniser->line.operand2,tokens[2]);
		copy_token(&tokeniser->line.operand3,tokens[3]);
		}
	
	return 0;
	
}

void free_tokeniser(Tokeniser* tokeniser) {
	free(tokeniser);
}
