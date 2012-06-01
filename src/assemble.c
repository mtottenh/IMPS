#include <stdlib.h>
#include <stdio.h>
#include "symbol_table.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        /*If not executed in the form ./assemble <input file> <output file> error and quit*/
        printf("\nUsage: %s <input file> <output file>\n", argv[0]);
        return EXIT_FAILURE;
    } else {
        /*If we got proper args, try to open the input file for reading*/
        FILE *input;
	 =;
	if ((input = fopen(argv[1],"r")) == NULL) {
            printf("Error: Could not open file: %s ", argv[1]);
            return EXIT_FAILURE;
        }

        /* Opens the output file to write to in binary mode. */
        char *output_path = argv[2];
        FILE *output;
        if ((output = fopen(output_path, "wb")) == NULL)
        {
                perror("Output file couldn't be opened");
                exit(EXIT_FAILURE);
        }


        /* 1. Read through the binary file and construct the symbol table*/
            /* 1.1 Tokenize a line and put into  a stuct? similar to what we did with operands?*/
            /* 1.2 Each line in an assembly file is 4 addresses starting from 0 (as byte adressable memory and each line is 32 bytes)*/
            /* 1.3 Every time we encounter a label. Add it to the symbol table and add its corresponding address*/
        /* 2. Read through the binary file and construct the binary output using symbol table*/
            /* 2.1 Consider creating 3 funcs to deal with the 3 different instruction formats?*/
        /* 3. Write binary output to the file specified by <output file>*/
            /* 3.1 What are we going to use to store the temp output of an assembler thing? just a char array of MEM_SIZE?*/
    }


    return EXIT_SUCCESS;
    
}
