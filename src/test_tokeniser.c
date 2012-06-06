#include <assert.h>
#include "tokeniser.h"

int main(int argc, char** argv) {

       	FILE *input;
	if ((input = fopen(argv[1],"rt")) == NULL) {
            printf("Error: Could not open file: %s ", argv[1]);
            return EXIT_FAILURE;
        }
	Tokeniser *t = NULL;
	tokeniser_init(input, &t);
	printf("Tokeniser created!\n");
	while(!get_tokenised_line(t))
	{
		printf("Operand 1 : %s\n", t->line.operand1);

	}

	return 0;
}


 
