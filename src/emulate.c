#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "emulate.h"

uint32_t convert(uint32_t val) {
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
	return (val << 16) | ( val >> 16);
}
	
int main(int argc, char **argv) {

	/*
	 * Allocate memory for our machine state. Terminate in the event of
	 * failure.
	 */
	State* current = malloc(sizeof(State));
	
	if (current == NULL)
	{
		perror("Couldn't allocate memory to state");
		return EXIT_FAILURE;
	}	

	/* Initialise our state and setup opcode function pointers. */
	init(current);
	FunctionPointer func_pointers[19] = {NULL};
	setup_pointers(func_pointers);

	FILE *binary_file;

	/* Get binary file path. */
	char *file_path = argv[1];

	/* Open binary file. */
	if((binary_file = fopen(file_path, "rb")) == NULL)
	{
		perror("Couldn't open file specified at runtime");
		return EXIT_FAILURE;
	}

	/* Read binary file into the current state's memory */
	fread(current->pc, sizeof(uint8_t), MEM_SIZE, binary_file);
	fclose(binary_file);

	//for (int i = 0; i < MEM_SIZE; i++ ) {
	//	current->mem[i] = convert(current->mem[i]);
//	}

	/* Begin decode execute loop */
	uint8_t opcode;
	
	do {
		uint32_t instruction = *((uint32_t*) current->pc);
		opcode = extract_opcode(instruction);
		printf("opcode = %u, instruction = %x\n", opcode, instruction);
		func_pointers[opcode](instruction, current);
	} while (opcode != 0);

	return EXIT_SUCCESS;
}

/* Initialises a machine state */
int init(State *machine_state) {
	/*
	 * Sets PC to first memory location
	 * PC is a pointer to 8 bits of memory;
	 * has to be byte addressable
	 */
	 machine_state->pc = machine_state->mem;

	 /* Initialise memory to 0 */
	 memset(machine_state->mem, 0, sizeof(uint8_t)*MEM_SIZE);
	 memset(machine_state->reg, 0, sizeof(uint32_t)*NUM_REGS);
	 return 0;
}

/*
 * Set up opcode-function pointer array lookup table.
 * The array index corresponds to the opcode, whilst its value is a pointer
 * to the function associated with that opcode.
 */
void setup_pointers(FunctionPointer array[]) {
	array[0] = &halt_instruction;
	array[1] = &add_instruction;
	array[2] = &addi_instruction;
	array[3] = &sub_instruction;
	array[4] = &subi_instruction;
	array[5] = &mul_instruction;
	array[6] = &muli_instruction;
	array[7] = &lw_instruction;
	array[8] = &sw_instruction;
	array[9] = &beq_instruction;
	array[10] = &bne_instruction;
	array[11] = &blt_instruction;
	array[12] = &bgt_instruction;
	array[13] = &ble_instruction;
	array[14] = &bge_instruction;
	array[15] = &jmp_instruction;
	array[16] = &jr_instruction;
	array[17] = &jal_instruction;
	array[18] = &out_instruction;
}

/* Increments the program counter by a given number of steps */
void increment_pc(State *machine_state, int16_t i) {
	machine_state->pc = machine_state->pc + (4 * i);
}

/* Extraction functions to split an instruction into its component parts. */
static uint8_t const START_OPCODE = 0;
static uint8_t const END_OPCODE = 5;
static uint8_t const REG_WIDTH = 5;
static uint8_t const END_INSTRUCTION = 31;

/*
 * General extraction function - extracts the bits located at a given start
 * and end location inclusive.
 */
uint32_t extract(uint32_t instruction, uint8_t start, uint8_t end) {
	uint32_t mask = 0;
	start = 31 - start; 
	end = 31 - end;

	for (int i = end ;i <= start; i++) {
		mask = mask + (1 << i);
	}
 	
	return (mask & instruction) >> (end);
}

/* Extacts the opcode from a given instruction via a call to extract. */
uint8_t extract_opcode(uint32_t instruction) {
	return (uint8_t)extract(instruction, START_OPCODE, END_OPCODE);
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
	operands.immediate = (int16_t)extract(instruction, immediate_start,
		 END_INSTRUCTION);
	
//	if ( extract(operands.immediate,0,0) ) {
//
//		operands.immediate &= 0x7FFF;
//		operands.immediate = -operands.immediate;
//	}

	printf("Immediate value: %d\n", operands.immediate);
	return operands;
}

/* Functions corresponding to the IMPS opcode functions. */

void halt_instruction(uint32_t instruction, State *machine_state) {
	/* Print the values of PC and registers, then terminate the program. */
	fprintf(stderr, "PC: %x\n", *(machine_state->pc));
	
	for(int i = 0; i < NUM_REGS; i++) {
		fprintf(stderr, "R%d: %x\n", i, machine_state->reg[i]);
	}	
}

void add_instruction(uint32_t instruction, State *machine_state) {
	/* Add the values of two given registers, storing them in the first. */
	OperandsR operands = extract_r(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 + machine_state->reg[operands.r3];
	increment_pc(machine_state, 1);	
}

void addi_instruction(uint32_t instruction, State *machine_state) {	
	/* Adds the value of a register and an immediate value, storing the
	 * result in another register. */
	OperandsI operands = extract_i(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 + operands.immediate;
	increment_pc(machine_state, 1);
}

void sub_instruction(uint32_t instruction, State *machine_state) {
	/* Performs subtraction of the values contained in two registers. */
	OperandsR operands = extract_r(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 - machine_state->reg[operands.r3];
	increment_pc(machine_state, 1);
}

void subi_instruction(uint32_t instruction, State *machine_state) {
	/* Performs subtraction of a register value and intermediate value. */
	OperandsI operands = extract_i(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 - operands.immediate;
	increment_pc(machine_state, 1);
}

void mul_instruction(uint32_t instruction, State *machine_state) {
	/* Multiplies the values of two registers. */
	OperandsR operands = extract_r(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 * machine_state->reg[operands.r3];
	increment_pc(machine_state, 1);
}

void muli_instruction(uint32_t instruction, State *machine_state) {
	/* Multiplies the value of a register and intermediate value. */
	OperandsI operands = extract_i(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2]
		 * operands.immediate;
	increment_pc(machine_state, 1);
}

void lw_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * Loads the value of memory at address [R2 + C] into R1,
	 * where R2 and R1 are register operands and C is an immediate value.
	 */
	OperandsI operands = extract_i(instruction);
	printf( "Reg[%d] : %d\t", operands.r1,machine_state->reg[operands.r1]);
        printf( " Reg[%d] : %d\t", operands.r2,machine_state->reg[operands.r2]);
	uint32_t r2 = machine_state->reg[operands.r2];	
	uint32_t *result = &machine_state->mem[r2 + operands.immediate];
	machine_state->reg[operands.r1] = *result;
	increment_pc(machine_state, 1);
}

void sw_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * Sets the value of memory at location [R2 + C] to be equal to the
	 * value of R1, where R2 and R1 are register operands and C is an
	 * immediate value.
	 */
	OperandsI operands = extract_i(instruction);
	uint32_t r2 = machine_state->reg[operands.r2];
 	uint32_t *pointer = &machine_state->mem[r2 + operands.immediate];
	*pointer  = machine_state->reg[operands.r1];
	increment_pc(machine_state, 1);
}

void beq_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * If the values of two register operands are equal, increment the
	 * program counter by 4 * C, where C is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */	
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] == machine_state->reg[operands.r2])
		increment_pc(machine_state, operands.immediate);
	else
		increment_pc(machine_state,1);
}

void bne_instruction(uint32_t instruction, State *machine_state) {	
	/*
	 * If the values of two register operands are not equal, increment the
	 * program counter by 4 * C, where C is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] != machine_state->reg[operands.r2])
		increment_pc(machine_state, operands.immediate);
	else
		increment_pc(machine_state,1);
}

void blt_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * If the value of the first register operand is less than that of
	 * the second operand, increment the program counter by 4 * C, where C
	 * is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] < machine_state->reg[operands.r2])
		 increment_pc(machine_state, operands.immediate);
	else
		increment_pc(machine_state,1);
}

void bgt_instruction(uint32_t instruction, State *machine_state) {	
	/*
	 * If the value of the first register operand is greater than that of
	 * the second operand, increment the program counter by 4 * C, where C
	 * is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] > machine_state->reg[operands.r2])
		 increment_pc(machine_state, operands.immediate);
	else
		increment_pc(machine_state,1);
}

void ble_instruction(uint32_t instruction, State *machine_state) {	
	/*
	 * If the value of the first register operand is less than or equal to
	 * that of second operand, increment the program counter by 4 * C, where
	 * C is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */			
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] <= machine_state->reg[operands.r2])
		 increment_pc(machine_state, operands.immediate);
	else
		increment_pc(machine_state,1);
}

void bge_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * If the value of the first register operand is grater than or equal to
	 * that of second operand, increment the program counter by 4 * C, where
	 * C is an immediate value.
	 * Otherwise, increment the program counter normally.
	 */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] >= machine_state->reg[operands.r2])
		 increment_pc(machine_state, operands.immediate);
	else	
		increment_pc(machine_state,1);
}

void jmp_instruction(uint32_t instruction, State *machine_state) {
	/* Jump to an address by setting the PC to a given operand. */
	uint32_t address = extract_address(instruction);
	machine_state->pc = &machine_state->mem[address];	
}

void jr_instruction(uint32_t instruction, State *machine_state) {
	/* 
	 * Jump to an address by setting the PC to the value of a given
	 * register operand.
	 */
	OperandsR operands = extract_r(instruction);
	printf("Jumping to: %x",machine_state->reg[operands.r1]);
	/* this is wrong, it should be jumping to 
	 * somewhere in memory not the address of a register!
	 *
	 *	machine_state->pc = &machine_state->reg[operands.r1];
	 * The desired behaviour is coded below!
	 */
	uint8_t address = machine_state->reg[operands.r1];
	machine_state->pc = &machine_state->mem[address];
}

void jal_instruction(uint32_t instruction, State *machine_state) {
	/*
	 * Store the instruction that would normally be executed next (PC + 4)
	 * in R31 and set PC to a given operand.
	 */
	uint8_t address = extract_address(instruction);
	machine_state->reg[31] = *(machine_state->pc + 4);
	machine_state->pc = &machine_state->mem[address];
}


void out_instruction(uint32_t instruction, State *machine_state) {
	/* Prints the least significant eight bits of R1 to stdout. */	
	OperandsR operands = extract_r(instruction);
	uint32_t regVal = machine_state->reg[operands.r1];
	uint32_t out = extract(regVal, END_INSTRUCTION - 7, END_INSTRUCTION);
	printf("**%x**", out);
	increment_pc(machine_state,1);	
}

/*useful code
 * for(int i = 0; i<MEM_SIZE; i++) {
		printf("current.mem[%d]  : %d\n", i , current.mem[i]);
	}*/

/* unuseful code
 ** far too complicated(/awesome) way of extracting bits from numbers...
*      if (start == 0) {
                start = 0xffffffff;
        } else {
                start =  31 - start;
                start = ((2 << (start )) -1);
        }
        end = 31 - end;
        end = ( 2 << (end ) ) -1;
        int result = 0;


        uint32_t mask = start - end;
*/
