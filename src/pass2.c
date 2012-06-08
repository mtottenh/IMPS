#include "pass2.h"

void pass2(FILE* input, FILE* output, Symbol_Table* table) {
	printf("****PASS 2****\n\n");
	printf("**Symbol Table Recieved**\n");
	Symbol_Table_print(table);
	printf("**End Table**\n");
	/* Create tokeniser. */
	Tokeniser* tokeniser;
	tokeniser_init(input, &tokeniser);

	/* Create array of function pointers. */
	FunctionPointer func_pointers[21] = {NULL};
	setup_pointers(func_pointers);

	/* Set up buffer and address for output. */
	uint32_t buffer[MEM_SIZE / 32];
	uint16_t address = 0;

	/* Set up instruction data struct. */
	Instruction instr_data;
	instr_data.table = table;
	instr_data.address = &address;

	/* Loop through the input file via the tokeniser until EOF or error occurs. */
	Tokeniser_Line line;
	uint32_t assembled_line;
	uint32_t opcode;

	while(get_tokenised_line(tokeniser) == 0) {
		/* Get the tokenised line and current opcode. */
		line = tokeniser->line;
		opcode = Symbol_Table_get(table, line.opcode)->value;

		/* Set up instruction struct for this line. */
		instr_data.opcode = opcode;
		instr_data.operand1 = line.operand1;
		instr_data.operand2 = line.operand2;
		instr_data.operand3 = line.operand3;

		/*
		 * Create an assembled line. If the instruction isn't a .skip
		 * directive, then add it to the buffer at the current address
		 * and increment it.
		 */
		assembled_line = func_pointers[opcode](instr_data);
		if (!(opcode == SKIP_OPCODE)) {
			buffer[address] = assembled_line;
			address++;
		}
	}

	/* Write buffer to output file. */
	fwrite(buffer, sizeof(uint32_t), address, output);

	/* Free memory of the tokeniser. */
	free_tokeniser(tokeniser);
}

void setup_pointers(FunctionPointer array[]) {
	/*
	 * Array of opcodes mapped to function pointers representing their 
	 * instruction type. The fill and skip directives have been arbitrarily
	 * assigned unused opcodes 19 and 20, as defined in symbol_table.c.
	 */
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

uint32_t eval_immediate(char* immediate, uint32_t opcode, Symbol_Table* table) {
	/* An immediate value can be a label reference or hex/decimal value. */
	
	uint32_t result = 0;

	if (immediate == NULL) {
		printf("\n******NULL PASSED TO EVAL IMMEDIATE RETURNING 0*****\n");
		return result;
	}

	/* Check if a mapping exists in the symbol table. */
	/*Again this needs to change as get returns a pointer to a (key,value) pair not an int*/
	Symbol_Table_Entry* label_entry = Symbol_Table_get(table, immediate);
	printf("Label:%s\t", immediate);
	if (label_entry != NULL) {
		result = (uint32_t) label_entry->value;
		printf("Label's Value in Table: %u\n", result);
	} else {
		/*
		 * If the value is hex, strtol() with base 0 will detect it as
		 * hex (due to prefix 0x), otherwise base 10.
		 */
		int32_t value = (int32_t) strtol(immediate, NULL, 0);
		result = (uint32_t) value;

	}
	
	
	result &= 0x0000FFFF; 
	//printf("Immediate Value: %u", result);
	return result;
}

uint32_t eval_register(char* regstring) {
	/* Increment pointer to remove $ character. */
	regstring++;
	printf("Reg string: %s\t", regstring);
 	return (uint32_t) atoi(regstring);
}

uint32_t assemble_halt(Instruction instruction) {
	/* Halt instruction is simply 32 zero bits. Return 0. */
	return 0;
}

uint32_t assemble_rtype(Instruction instruction) {
	/* All operands are registers. Get R1, R2, R3... */
	uint32_t result = instruction.opcode << 26;


	printf("OP1: %s, OP2: %s, OP3: %s", instruction.operand1, instruction.operand2, instruction.operand3);
	char* operand1 = instruction.operand1;
	if (operand1 != NULL) {
		result |= (eval_register(operand1) << 21);
	}

	char* operand2 = instruction.operand2;
	if (operand2 != NULL) {
		result |= (eval_register(operand2) << 16);
	}

	char* operand3 = instruction.operand3;
	if (operand3 != NULL) {
		result |= (eval_register(operand3) << 11);
	}

	return result;
}

uint32_t assemble_itype(Instruction instruction) {
	/* Is it a branch instruction? */
	if (instruction.opcode >= START_BRANCH 
		&& instruction.opcode <= END_BRANCH) {
		return assemble_branch(instruction);
	}

	/* Operands 1 and 2 are registers. Get R1 and R2. */
	uint32_t result = instruction.opcode << 26;

	char* operand1 = instruction.operand1;
	result |= (eval_register(operand1) << 21);

	char* operand2 = instruction.operand2;
	result |= (eval_register(operand2) << 16);

	/* Operand 3 is an immediate value. */
	char* operand3 = instruction.operand3;

	printf("\nOP1: %s\tOP2: %s\tOP3: %s\n", instruction.operand1,instruction.operand2,instruction.operand3);
	uint32_t result2 = eval_immediate(operand3, instruction.opcode, instruction.table);
	printf("Immediate value for I type: %u", result2);
	return result |= result2;
}

uint32_t assemble_branch(Instruction instruction) {
	/* Operands 1 and 2 are registers. Get R1 and R2. */
	uint32_t opcode = instruction.opcode;
	uint32_t result = opcode << 26;

	char* operand1 = instruction.operand1;
	result |= (eval_register(operand1) << 21);

	char* operand2 = instruction.operand2;
	result |= (eval_register(operand2) << 16);

	/* Operand 3 is an offset from the current address. */
	char* operand3 = instruction.operand3;
	uint32_t offset = eval_immediate(operand3, opcode, instruction.table);

	/*
	 * Divide offset by 4 as we're dealing with words, and subtract the 
	 * current address. 
	 */
	offset /= 4;
	offset -= *(instruction.address);
	result |= offset;

	return result;
}

uint32_t assemble_jtype(Instruction instruction) {
	/* One operand, which is an absolute immediate address. */
	uint32_t result = instruction.opcode << 26;
	char* operand1 = instruction.operand1;
	
	uint32_t result2 = eval_immediate(operand1, instruction.opcode, instruction.table);
	printf("\tJump Location: %u\n", result2);
	return result |= result2;
}

uint32_t assemble_fill(Instruction instruction) {
	/* Get operand 1, convert to an integer and return. */
	char* operand1 = instruction.operand1;

	return (uint32_t) atoi(operand1);
}

uint32_t assemble_skip(Instruction instruction) {
	int num_reserve = atoi(instruction.operand1);

	/* Reserve n words (32 bits of 0). */
	for(int i = 0; i < num_reserve; i++) {
		/* Cannot use ++ operator when dereferencing! */
		*(instruction.address) += 1;
	}

	return 0;
}
