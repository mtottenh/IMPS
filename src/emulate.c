#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include "emulate.h"

int main(int argc, char **argv) {
	state current;
	//Need to make state allocated on heap rather than on stack!
	init(&current);
	functionPointer funcPointers[19] = {NULL};
	setup_pointers(funcPointers);
	printf("*****extract_opcode TEST*****\n");
	printf("\nopcode: %x\n",extract_opcode(0xFC000000));
	printf("input: %x\n", 0xFC000000);

	printf("*****extract TEST*****\n");
	printf("extracted: %x\n", extract(0xFC000000,0,5));
	/*Read contents of binary file into state*/
		/*Read binary file 32 *BITS* At a time*/
		/*Convert the 32 bits/chars into a uint32_t*/
		/*store each uint32_t into state.mem*/
	/*Start decode execute loop*/
		/*Finish if halt encountered*/
	return 0;
}

/*initialises a machine state*/
int init(state *machine_state) {
	/*
	 * Sets PC to first memory location
	 * PC is a pointer to 8 bits of memory;
	 * has to be byte addressable
	 */
	 machine_state->pc = (uint8_t *)machine_state->mem;

	 /* Initialise memory to 0 - see man 3 memset */
	 memset(machine_state->mem, 0, sizeof(uint32_t)*MEM_SIZE);
	 memset(machine_state->reg, 0, sizeof(uint32_t)*NUM_REGS);
	 return 0;
}

void setup_pointers(functionPointer array[]) {
	array[0] = &halt_instruction;
	array[1] = &add_instruction;
	array[2] = &addi_instruction;
	array[3] = &sub_instruction;
	array[4] = &subi_instruction;
	array[5] = &mul_instruction;
	array[6] = &subi_instruction;
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

void increment_pc(state *machine_state) {
	machine_state->pc = machine_state->pc + 4;
}

/*extract opcode*/
#define START_OPCODE 0
#define END_OPCODE 5
#define REG_WIDTH 5
#define END_INSTRUCTION 31

uint32_t extract(uint32_t instruction, uint8_t start, uint8_t end) {

	uint32_t mask = 0;
	start = 31 - start; 
	end = 31 - end;

	for (int i = end ;i <= start; i++) {
		mask = mask + (1 << i);
	}
	printf("\nmask: %x\n", mask); 	
	return (mask & instruction) >> (end);
}

uint8_t extract_opcode(uint32_t instruction) {

	return (uint8_t)extract(instruction,START_OPCODE, END_OPCODE);
}

uint32_t extract_address(uint32_t instruction) {
	return extract(instruction, (END_OPCODE + 1), END_INSTRUCTION);
}

operandsR extractR(uint32_t instruction) {
	operandsR operands;

	uint8_t r1Start = END_OPCODE + 1;
	uint8_t r2Start = r1Start + REG_WIDTH;
	uint8_t r3Start = r2Start + REG_WIDTH;

	operands.r1 = extract(instruction, r1Start, r2Start - 1);
	operands.r2 = extract(instruction, r2Start, r3Start - 1);
	operands.r3 = extract(instruction, r3Start, r3Start + REG_WIDTH - 1);

	return operands;
}

operandsI extractI(uint32_t instruction) {
	operandsI operands;

	uint8_t r1Start = END_OPCODE + 1;
	uint8_t r2Start = r1Start + REG_WIDTH;
	uint8_t immediateStart = r2Start + REG_WIDTH;

	operands.r1 = extract(instruction, r1Start, r2Start - 1);
	operands.r2 = extract(instruction, r2Start, immediateStart - 1 );
	operands.immediate = extract(instruction, immediateStart, END_INSTRUCTION);

	return operands;
}


/*
uint8_t extract_register_index(uint32_t instruction, uint8_t number) {
	return (uint8_t)extract(instruction, (END_OPCODE + 1) + REG_WIDTH * (number - 1),		 (END_OPCODE + 1) + REG_WIDTH * (number));		
}


int16_t extract_immediate(uint32_t instruction) {
	return (int16_t)extract(instruction, (END_OPCODE + 1) + 2*REG_WIDTH, END_INSTRUCTION);
	//Need to implement a sign extension
}
*/ 


/* functions need to return failure/success */

void halt_instruction(uint32_t instruction, state *machine_state) {
	// Terminate by exiting the program
	// Dump registers to stderr first...
	fprintf(stderr, "PC: %x", *(machine_state->pc));
	
	for(int i = 0; i < NUM_REGS; i++) {
		fprintf(stderr, "R%d: %x", i, machine_state->reg[i]);
	}	
	
	exit(EXIT_SUCCESS);
}

void add_instruction(uint32_t instruction, state *machine_state){
	operandsR operands = extractR(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] + machine_state->reg[operands.r3];	
}

/* functions need to return failure/success */
void addi_instruction(uint32_t instruction, state *machine_state){	
	operandsI operands = extractI(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] + operands.immediate;
}

void sub_instruction(uint32_t instruction, state *machine_state){
	operandsR operands = extractR(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] - machine_state->reg[operands.r3];
}

void subi_instruction(uint32_t instruction, state *machine_state){
	operandsI operands = extractI(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] - operands.immediate;
}

void mul_instruction(uint32_t instruction, state *machine_state){
	operandsR operands = extractR(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] * machine_state->reg[operands.r3];
}

void muli_instruction(uint32_t instruction, state *machine_state){
	operandsI operands = extractI(instruction);
	machine_state->reg[operands.r1] = machine_state->reg[operands.r2] * operands.immediate;
}

void lw_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
	uint32_t result = machine_state->mem[operands.r2 + operands.immediate];
	machine_state->reg[operands.r1] = result;
}

void sw_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
 	machine_state->mem[operands.r2 + operands.immediate] = machine_state->reg[operands.r1];
}

void beq_instruction(uint32_t instruction, state *machine_state) {	
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] == machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void bne_instruction(uint32_t instruction, state *machine_state) {	
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] != machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void blt_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] < machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void bgt_instruction(uint32_t instruction, state *machine_state) {	
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] > machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void ble_instruction(uint32_t instruction, state *machine_state) {	
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] <= machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void bge_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
	if (machine_state->reg[operands.r1] >= machine_state->reg[operands.r2])
		 increment_pc(machine_state);
}

void jmp_instruction(uint32_t instruction, state *machine_state) {
	uint8_t address = extract_address(instruction);
	*(machine_state->pc) = address;	
}

void jr_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
	uint8_t r1Val = machine_state->reg[operands.r1];
	*(machine_state->pc) = r1Val;
}

void jal_instruction(uint32_t instruction, state *machine_state) {
	uint8_t address = extract_address(instruction);
	machine_state->reg[30] = *(machine_state->pc) + 4;
	*(machine_state->pc) = address;
}

void out_instruction(uint32_t instruction, state *machine_state) {
	operandsI operands = extractI(instruction);
	printf("%u/n", extract(machine_state->reg[operands.r1], END_INSTRUCTION - 7, END_INSTRUCTION));
	// Is this meant to print the integer value, or actually a sequence of bits?
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
