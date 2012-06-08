#include "tokeniser.h"

/* 
 * Allocates space for and returns an initialised Tokeniser, with
 * a FILE input source.
 */
Tokeniser* tokeniser_new(FILE* source) {
	Tokeniser* tokeniser = malloc(sizeof(Tokeniser));
	tokeniser->file = source;
	tokeniser->line.label = malloc(sizeof(char*));
	tokeniser->line.opcode = malloc(sizeof(char*));
	tokeniser->line.operand1 = malloc(sizeof(char*));
	tokeniser->line.operand2 = malloc(sizeof(char*));
	tokeniser->line.operand3 = malloc(sizeof(char*));

	if ((tokeniser->line.label == NULL) |
	    (tokeniser->line.opcode == NULL) |
	    (tokeniser->line.operand1 == NULL) |
	    (tokeniser->line.operand2 == NULL) |
	    (tokeniser->line.operand3 == NULL)) {
		perror("An element of tokeniser->line couldn't be allocated");
		exit(EXIT_FAILURE);
	}
	return tokeniser;
}

/* 
 * Allocates dest on the heap and copies the value of src into dest.
 * Assumes that *dest is already allocated on the heap and reallocates
 * space for the new value accordingly
 */
void copy_token(char **dest, const void *src) {
	if (src == NULL) {
		*dest = NULL; 
	} else {
		/* Reallocate memory and clear out previous data. */
		*dest = realloc(*dest, strlen(src));
		if (*dest == NULL) {
			perror("Couldn't reallocate memory for *dest");
			exit(EXIT_FAILURE);
		}		
		memset(*dest, '\0', sizeof(src));
		strncpy(*dest, src, strlen(src));
	}
}

int get_tokenised_line(Tokeniser* tokeniser) {
	char buffer[BUFFER_SIZE];
	memset(buffer, '\0', sizeof(buffer));

	/*
	 * Get the first BUFFER_SIZE - 1 characters from tokeniser->file and
	 * store them in buffer
	 */
	if((fgets(buffer, BUFFER_SIZE, tokeniser->file)) == NULL) {
		return EXIT_FAILURE;
	}
	
	/* 
	 * If a new line is encountered, ignore and 
	 * recurse with the next line
	 */
	if (buffer[0] == '\n') {
		return get_tokenised_line(tokeniser);
	}

	char* state = NULL;
	char* tokens[5];
	memset(tokens, '\0', sizeof(tokens));
	char* buffer_ptr = buffer;
	char *token = strtok_r(buffer_ptr, " \n\t", &state);
	
	/*
	 * While a next token exists and there is less than or equal to
	 * five tokens stored, store next token into token array.
	 */
	for (int i = 0; (token  != NULL) && i <= 4; i++) {
		tokens[i] = token;
		if (buffer_ptr != NULL) {
			buffer_ptr = NULL;
		}
		token = strtok_r(buffer_ptr, " \n\t", &state);
	}
	
	/* 
	 * Check if first token contains a ':' character. If so, it is
	 * a label so store the tokenised value with no offset.
	 * Otherwise, set the label field to NULL and offset the values.
	 */
	if (strchr(tokens[0], ':') != NULL) {
		copy_token(&tokeniser->line.label, tokens[0]);
		copy_token(&tokeniser->line.opcode, tokens[1]);
		copy_token(&tokeniser->line.operand1, tokens[2]);
		copy_token(&tokeniser->line.operand2, tokens[3]);
		copy_token(&tokeniser->line.operand3, tokens[4]);
	} else {
		tokeniser->line.label = NULL;
		copy_token(&tokeniser->line.opcode, tokens[0]);
		copy_token(&tokeniser->line.operand1, tokens[1]);
		copy_token(&tokeniser->line.operand2, tokens[2]);
		copy_token(&tokeniser->line.operand3, tokens[3]);
		}
	return EXIT_SUCCESS;
}

void free_tokeniser(Tokeniser* tokeniser) {
	free(tokeniser->line.label);
	free(tokeniser->line.opcode);
	free(tokeniser->line.operand1);
	free(tokeniser->line.operand2);
	free(tokeniser->line.operand3);
	free(tokeniser);
}
