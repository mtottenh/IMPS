typedef struct tokeniser {
	FILE *file;
	tokeniser_line line;
};

typedef struct tokenised_line {
	char* label;
	char* opcode;
	char* operand1;
	char* operand2;
	char* operand3;
};

void tokeniser_init(FILE*, tokeniser*);
int get_tokenised_line(tokeniser*);
void free_tokeniser(tokeniser*);
