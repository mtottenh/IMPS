#include <assert.h>
#include "tokeniser.h"

int main(int argc, char** argv) {

       	FILE *input;
	if ((input = fopen(argv[1],"rt")) == NULL) {
            printf("Error: Could not open file: %s ", argv[1]);
            return EXIT_FAILURE;
        }
	Tokeniser t;
	tokeniser_init(input, &t);
	while(!get_tokenised_line(&t))
	{
		printf("%s", t.line.opcode);
	}

	return 0;
}


 
