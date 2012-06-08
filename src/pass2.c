#include "pass2.h"

void pass2(FILE* input, FILE* output, Symbol_Table* table) {
	printf("****PASS 2****\n\n");
	printf("**Symbol Table Recieved**\n");
	symbol_table_print(table);
	printf("**End Table**\n");
	/* Create tokeniser. */
	Tokeniser* tokeniser;
	tokeniser_init(input, &tokeniser);

	/* Create array of function pointers. */
	FunctionPointer func_pointers[21] = {NULL};
	setup_pointers(func_pointers);

	/* Setup buffer for output. */
	uint32_t buffer[2048];

	/* Loop through the input file via the tokeniser until EOF or error occurs. */
	Tokeniser_Line line;
	uint32_t assembled_line;
	uint32_t opcode;
	uint16_t index = 0;

	while(get_tokenised_line(tokeniser) == 0) {
		/* Get the tokenised line and given opcode. */
		line = tokeniser->line;
		opcode = symbol_table_get(table,line.opcode)->value;
		printf("Opcode: %u\t", opcode);

		/* Special case! Opcode 20 = skip. */
		switch(opcode) {
			case 20:
			{
				 char* operand1 = line.operand1;
				
				/* Reserve n words (32 bits of 0). */
				for(int i = 0; i < atoi(operand1); i++) {
					//buffer[index] = 0;
					index++;
				}			
			}
			break;
			case 9:
			case 10:
			case 11:
			case 12: 
			case 13:
			case 14:
			{
				
			        uint32_t offset = eval_immediate(line.operand3,opcode,table);
				printf("\nBranch instruction detected\n");
				printf("Index: %x\t Offset w/o index: %x\t", index, offset);
				offset /= 4;
				offset -= index;
				assembled_line = opcode << 26;
				assembled_line |= (eval_register(line.operand1) << 21);
				assembled_line |= (eval_register(line.operand2) << 16);
				assembled_line |= offset;
				printf("Offset: %x\tAssembled Line: %x\n", offset,assembled_line);
				buffer[index] = assembled_line;
				index++;
			}
			break;
			default: 
			{
				/*
			 	 * Get the assembled line and add it to the line at the 
		 		 * current index of the buffer.
				 */
				assembled_line = 
					func_pointers[opcode](opcode, line, table);
				buffer[index] = assembled_line;
				index++;
				printf("\nAssembled line: %x\n", assembled_line);
			}
			break;
		}
		printf(	"***NEXT INSTRUCTION***\n");


	}

	/* Write buffer to output file. */
	fwrite(buffer, sizeof(uint32_t), index, output);
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
	Symbol_Table_Entry* label_entry = symbol_table_get(table, immediate);
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
	//printf("Reg string: %s\t", regstring);
 	return (uint32_t) atoi(regstring);
}

uint32_t assemble_halt(uint32_t opcode, Tokeniser_Line line, 
	Symbol_Table* table) {
	/* Halt instruction is simply 32 zero bits. Return 0. */
	return 0;
}

uint32_t assemble_rtype(uint32_t opcode, Tokeniser_Line line,
	 Symbol_Table* table) {
	/* All operands are registers. Get R1, R2, R3... */
	uint32_t result = opcode << 26;


	printf("OP1: %s, OP2: %s, OP3: %s", line.operand1, line.operand2, line.operand3);
	char* operand1 = line.operand1;
	if (operand1 != NULL) {
		result |= (eval_register(operand1) << 21);
	}

	char* operand2 = line.operand2;
	if (operand2 != NULL) {
		result |= (eval_register(operand2) << 16);
	}

	char* operand3 = line.operand3;
	if (operand3 != NULL) {
		result |= (eval_register(operand3) << 11);
	}

	return result;
}

uint32_t assemble_itype(uint32_t opcode, Tokeniser_Line line,
	 Symbol_Table* table) {
	/* Operands 1 and 2 are registers. Get R1 and R2. */
	uint32_t result = opcode << 26;

	char* operand1 = line.operand1;
	result |= (eval_register(operand1) << 21);

	char* operand2 = line.operand2;
	result |= (eval_register(operand2) << 16);

	/* Operand 3 is an immediate value. */
	char* operand3 = line.operand3;

	printf("\nOP1: %s\tOP2: %s\tOP3: %s\n", line.operand1,line.operand2,line.operand3);
	uint32_t result2 = eval_immediate(operand3, opcode, table);
	printf("Immediate value for I type: %u", result2);
	return result |= result2;
}

uint32_t assemble_jtype(uint32_t opcode, Tokeniser_Line line,
	 Symbol_Table* table) {
	/* One operand, which is an absolute immediate address. */
	uint32_t result = opcode << 26;
	char* operand1 = line.operand1;
	
	
	uint32_t result2 = eval_immediate(operand1, opcode, table);
	printf("\tJump Location: %u\n", result2);
	return result |= result2;
}

uint32_t assemble_fill(uint32_t opcode, Tokeniser_Line line, 
	Symbol_Table* table) {
	/* Get operand 1, convert to an integer and return. */
	char* operand1 = line.operand1;

	return (uint32_t) atoi(operand1);
}

