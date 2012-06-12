#include "pass2.h"

void pass2(FILE* input, FILE* output, Symbol_Table* table) {
	/* Create tokeniser. */
	Tokeniser* tokeniser = tokeniser_new(input);

	/* Create array of function pointers. */
	FunctionPointer func_pointers[21] = {NULL};
	setup_pointers(func_pointers);

	/* Set up buffer and address for output. */
	uint32_t buffer[MEM_SIZE / INSTR_WIDTH];
	uint16_t address = 0;

	/* Set up instruction data struct. */
	Instruction instr_data;
	instr_data.table = table;
	instr_data.address = &address;

	/* Loop through the input file via the tokeniser until EOF. */
	Tokeniser_Line line;
	uint32_t assembled_line;

	while(get_tokenised_line(tokeniser) == 0) {
		/* Get the tokenised line. */
		line = tokeniser->line;

		/* Set up instruction struct for this line. */
		instr_data.opcode = symbol_table_get(table, line.opcode)->value;
		instr_data.operand1 = line.operand1;
		instr_data.operand2 = line.operand2;
		instr_data.operand3 = line.operand3;

		/*
		 * Create an assembled line. If the instruction isn't a .skip
		 * directive, then add it to the buffer at the current address
		 * and increment it.
		 */
		assembled_line = func_pointers[instr_data.opcode](instr_data);
		if (!(instr_data.opcode == SKIP_OPCODE)) {
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
	array[21] = &assemble_stype; /* push */
	array[22] = &assemble_stype; /* pop */
	array[23] = &assemble_stype; /* call */
	array[24] = &assemble_stype; /* ret */
}

uint32_t eval_immediate(char* immediate, Symbol_Table* table) {
	/* An immediate value can be a label reference or hex/decimal value. */
	uint32_t result = 0;

	if (immediate == NULL) {
		return result;
	}

	/* Check if a mapping exists in the symbol table. */
	Symbol_Table_Entry* label_entry = symbol_table_get(table, immediate);
	
	if (label_entry != NULL) {
		result = (uint32_t) label_entry->value;
	} else {
		/*
		 * If the value is hex, strtol() with base 0 will detect it as
		 * hex (due to prefix 0x), otherwise base 10.
		 */
		result = (uint32_t) strtol(immediate, NULL, 0);
	}
	
	/* Perform bitwise AND on the result to extract the last 16 bits. */	
	result &= 0xFFFF; 

	return result;
}

uint32_t eval_register(char* regstring) {
	/* Increment pointer to remove $ character. */
	regstring++;

 	return (uint32_t) atoi(regstring);
}

uint32_t eval_stype(Symbol_Table* table, char* operand, uint32_t* flags) {
	/* Does the operand start with $? (Register). */
	if (operand[0] == '$') {
		return eval_register(operand);
	}
	
	/* Otherwise, evaluate the immediate value, and set immediate flag. */
	else {
		*flags |= STYPE_IMMEDIATE_FLAG;
		return eval_immediate(operand, table);
	}
}

uint32_t assemble_halt(Instruction instruction) {
	/* Halt instruction is simply 32 zero bits. Return 0. */
	return 0;
}

uint32_t assemble_rtype(Instruction instruction) {
	/* All operands are registers. Get R1, R2, R3... */
	int shift = INSTR_WIDTH;
	uint32_t result = instruction.opcode << (shift -= OPCODE_WIDTH);

	if (instruction.operand1 != NULL) {
		result |= (eval_register(instruction.operand1) 
				<< (shift -= REG_WIDTH));
	}

	if (instruction.operand2 != NULL) {
		result |= (eval_register(instruction.operand2) 
				<< (shift -= REG_WIDTH));
	}

	if (instruction.operand3 != NULL) {
		result |= (eval_register(instruction.operand3) 
				<< (shift -= REG_WIDTH));
	}

	return result;
}

uint32_t assemble_itype(Instruction instruction) {
	/* Is it a branch instruction? If so, delegate to assemble_branch. */
	if (instruction.opcode >= START_BRANCH 
		&& instruction.opcode <= END_BRANCH) {
		return assemble_branch(instruction);
	}

	/* Operands 1 and 2 are registers. Get R1 and R2. */
	int shift = INSTR_WIDTH;
	uint32_t result = instruction.opcode << (shift -= OPCODE_WIDTH);

	result |= (eval_register(instruction.operand1) << (shift -= REG_WIDTH));

	result |= (eval_register(instruction.operand2) << (shift -= REG_WIDTH));

	/* Operand 3 is an immediate value. */
	result |= eval_immediate(instruction.operand3, instruction.table);
	
	return result;
}

uint32_t assemble_branch(Instruction instruction) {
	/* Operands 1 and 2 are registers. Get R1 and R2. */
	int shift = INSTR_WIDTH;
	uint32_t result = instruction.opcode << (shift -= OPCODE_WIDTH);

	result |= (eval_register(instruction.operand1) << (shift -= REG_WIDTH));

	result |= (eval_register(instruction.operand2) << (shift -= REG_WIDTH));

	/* Operand 3 is an offset from the currenress. */
	uint32_t offset = eval_immediate(instruction.operand3, instruction.table);

	/*
	 * Convert offset into words as we're dealing with words, and subtract 
	 * the current address. 
	 */
	offset /= INSTR_WIDTH / 8;
	offset -= *(instruction.address);
	result |= offset;

	return result;
}

uint32_t assemble_jtype(Instruction instruction) {
	/* One operand, which is an absolute immediate address. */
	uint32_t result = instruction.opcode << (INSTR_WIDTH - OPCODE_WIDTH);
	
	result |= eval_immediate(instruction.operand1, instruction.table);
	
	return result;
}

uint32_t assemble_stype(Instruction instruction) {
	int shift = INSTR_WIDTH;
	uint32_t result = instruction.opcode << (shift -= OPCODE_WIDTH);

	/* Set up flags, initially 0x0. */ 
	uint32_t flags = 0x0;

	/* Is the first operand a memory access? (in the form [...]) */
	char* operand1 = instruction.operand1;
	if (operand1[0] == '[' && operand1[strlen(operand1) - 1] == ']') {
		/*
		 * Remove the [ and ] characters by incrementing the pointer
		 * and setting the final character to the \0. Set mem flag.
		 */
		flags |= STYPE_MEM_FLAG;
		operand1++;
		operand1[strlen(operand1) - 1] = '\0';
	}

	/* Get the value of the operand. */
	uint32_t operand_value = eval_stype(instruction.table, operand1, &flags);

	result |= flags << (shift -= STYPE_FLAGS_WIDTH);
	result |= operand_value;

	return result;
}

uint32_t assemble_fill(Instruction instruction) {
	/* Get operand 1, convert to an integer and return. */
	return (uint32_t) atoi(instruction.operand1);
}

uint32_t assemble_skip(Instruction instruction) {
	int num_reserve = atoi(instruction.operand1);

	/* Reserve n words by incrementing the address. */
	for(int i = 0; i < num_reserve; i++) {
		/* Cannot use ++ operator when dereferencing a pointer! */
		*(instruction.address) += 1;
	}

	return 0;
}
