#include "utils.h"

/* Checks whether a given opcode is valid. */
int is_valid_opcode(uint8_t opcode) {
	return opcode >= 0 && opcode < NUM_OPCODES;
}

/* Checks whether access to a memory location is valid. If not, terminate. */
int check_mem_access(int location) {
	if (location < 0 || location > MEM_SIZE - 4) {
		fprintf(stderr, "*** Cannot access memory "
			"location '%d'.\n", location);
		return 1;
	}
	return 0;
}

/* Checks whether an address is valid. If not, terminate. */
int check_address(uint32_t address) {
	if(address >= (8 * MEM_SIZE)) {
		fprintf(stderr, "*** Cannot access memory "
			"address '%u'. \n", address);
		return 1;
	}
	return 0;
}

/*
 * General extraction function - extracts the bits located at a given start
 * and end location inclusive.
 */
uint32_t extract(uint32_t instruction, uint8_t start, uint8_t end) {
	/* Set initial empty mask. */
	uint32_t mask = 0;

	/* Adjust start and end points for little endian conversion. */
	start = 31 - start; 
	end = 31 - end;

	for (int i = end; i <= start; i++) {
		mask += (1 << i);
	}
 	
	return (mask & instruction) >> end;
}

/* Extacts the opcode from a given instruction via a call to extract. */
uint8_t extract_opcode(uint32_t instruction) {
	uint8_t opcode = (uint8_t)extract(instruction, START_OPCODE, END_OPCODE);
	return opcode;
}

/* Extracts the address of a given instruction via a call to extract. */
uint32_t extract_address(uint32_t instruction) {
	return extract(instruction, (END_OPCODE + 1), END_INSTRUCTION);
}

/*
 * Given a bit pattern corresponding to an R-Type instruction, this function
 * returns a struct containing the R1, R2 and R3 components of the instruction.
 */
OperandsR extract_r(uint32_t instruction) {
	OperandsR operands;

	uint8_t r1_start = END_OPCODE + 1;
	uint8_t r2_start = r1_start + REG_WIDTH;
	uint8_t r3_start = r2_start + REG_WIDTH;
	
	operands.r1 = extract(instruction, r1_start, r2_start - 1);
	operands.r2 = extract(instruction, r2_start, r3_start - 1);
	operands.r3 = extract(instruction, r3_start, r3_start + REG_WIDTH - 1);

	return operands;
}

/*
 * Given a bit pattern corresponding to an I-Type instruction, this function
 * returns a struct containing the R1, R2 and intermediate value components
 * of the instruction.
 */
OperandsI extract_i(uint32_t instruction) {
	OperandsI operands;

	uint8_t r1_start = END_OPCODE + 1;
	uint8_t r2_start = r1_start + REG_WIDTH;
	uint8_t immediate_start = r2_start + REG_WIDTH;

	operands.r1 = extract(instruction, r1_start, r2_start - 1);
	operands.r2 = extract(instruction, r2_start, immediate_start - 1 );
	
	/* Casting to int16_t performs sign extension if necessary. */
	operands.immediate = (int16_t)extract(instruction, immediate_start,
		 END_INSTRUCTION);

	return operands;
}
