#include "pass2.h"

void pass2(FILE* output, Symbol_Table* table); {
	/* Create array of function pointers. */
	FunctionPointer func_pointers[21] = {NULL};
	setup_pointers(func_pointers);

	/* Setup buffer for output. */
	uint32_t buffer[2048];

	/* Declare memory for operands struct. */
	Operands operands = malloc(sizeof(Operands));

	/* Loop through the input file via the tokeniser until EOF or error occurs. */
	Tokeniser_Line* line;
	uint32_t assembled_line;
	uint32_t opcode;
	uint16_t index = 0;

	while(get_tokenised_line(tokeniser) == 0) {
		/* Get the tokenised line and given opcode. */
		line = tokeniser->line;
		opcode = (uint32_t) symbol_table_get(table, line->opcode);

		/*
		 * Get the assembled line and add it to the line at the 
		 * current index of the buffer.
		 */
		assembled_line = (opcode << 25) | func_pointers[opcode](line);
		buffer[index] = assembled_line;
		index++;
	}

	/* Deallocate memory from operands struct. */
	free(operands);
}

void setup_pointers(FunctionPointer array[]) {
	array[0] = &assemble_halt; /* halt */
	array[1] = &assemble_rtype; /* add */
	array[2] = &assemble_itype; /* addi */
	array[3] = &assemble_rtype; /* sub */
	array[4] = &assemble_itype; /* subi */
	array[5] = &assemble_rtype; /* mul */
	array[6] = &assemble_itype; /* muli */
	array[7] = &assemble_itype; /* lw */
	array[8] = &assemble_itype; /* sw */
	array[9] = &assemble_itype; /* beq */
	array[10] = &assemble_itype; /* bne */
	array[11] = &assemble_itype; /* blt */
	array[12] = &assemble_itype; /* bgt */
	array[13] = &assemble_itype; /* ble */
	array[14] = &assemble_itype; /* bge */
	array[15] = &assemble_jtype; /* jmp */
	array[16] = &assemble_rtype; /* jr */
	array[17] = &assemble_jtype; /* jal */
	array[18] = &assemble_rtype; /* out */
	array[19] = &assemble_fill; /* .fill directive */
	array[20] = &assemble_skip; /* .skip directive */
}

int16_t eval_immediate(char* immediate) {

}

uint32_t eval_register(char* regstring) {
	regstring++; /* increment pointer by 1 to remove $ character. */
	return (uint32_t) atoi(regstring);
}

uint32_t assemble_halt(Tokeniser_Line* line) {
	return 0;
}

uint32_t assemble_rtype(Tokeniser_Line* line) {
	/* All operands are registers. Get R1, R2, R3... */
	uint32_t result = 0;

	char* operand1 = line->operand1;
	if (operand1 != NULL) {
		result |= (eval_register(operand1) << 21);
		return result;
	}

	char* operand2 = line->operand2;
	if (operand2 != NULL) {
		result |= (eval_register(operand2) << 16);
		return result;
	}

	char* operand3 = line->operand3;
	if (operand3 != NULL) {
		result |= (eval_register(operand3) << 11);
	}

	return result;
}

uint32_t assemble_itype(Tokeniser_Line* line) {

}	
