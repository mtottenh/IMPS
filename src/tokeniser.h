#ifndef _TOKENISER_H_
#define _TOKENISER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Tokeniser_Line {
	char* label;
	char* opcode;
	char* operand1;
	char* operand2;
	char* operand3;
} Tokeniser_Line;

typedef struct Tokeniser {
	FILE *file;
	Tokeniser_Line line;
} Tokeniser;

// Definition for strtok_r, not defined in C99. See man strtok_r for details
char* strtok_r(char*, const char*, char**);

Tokeniser* tokeniser_new(FILE*);
void copy_token(char**, const void*);
int get_tokenised_line(Tokeniser*);
void free_tokeniser(Tokeniser*);

#endif
