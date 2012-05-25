#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#define MEM_SIZE 16384
#define NUM_REGS 32


typedef struct state {
	uint8_t *pc;
	uint32_t mem[MEM_SIZE];
	uint32_t reg[NUM_REGS];
} state;

int init(state*);
uint8_t extract_opcode(uint32_t);
uint32_t extract(uint32_t,uint8_t,uint8_t);
int main(int argc, char **argv) {
	state current;
	init(&current);
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
		mask = mask + ( 1 << i);
	}
	printf("\nmask: %x\n", mask); 	
	return (mask & instruction) >> (end);
}

uint8_t extract_opcode(uint32_t instruction) {
	
	return extract(instruction,START_OPCODE, END_OPCODE);
}

uint8_t extractRegisterIndex(uint32_t instruction, uint8_t number) {
	return extract(instruction, (END_OPCODE + 1) + REG_WIDTH * (number - 1),		 (END_OPCODE + 1) + REG_WIDTH * (number));		
}

uint32_t extractAddress(uint32_t instruction) {
	return extract(instruction, (END_OPCODE + 1), END_INSTRUCTION);
}

/*
Need a signed extension extraction function
*/





/* functions need to return failure/success */

void halt_function(uint32_t instruction, state *machine_state) {
	//To Do!
}

void incrementPC(state *machine_state) {
	machine_state->pc = machine_state->pc + 4;
}

void add_function(uint32_t instruction, state *machine_state){
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	uint8_t r3 = extractRegisterIndex(instruction, 3);

	machine_state->reg[r1] = machine_state->reg[r2] + machine_state->reg[r3];	
}

/* functions need to return failure/success */
void addi_function(uint32_t instruction, state *machine_state){	
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);

	machine_state->reg[r1] = machine_state->reg[r2] + immediate;
}

void sub_function(uint32_t instruction, state *machine_state){
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	uint8_t r3 = extractRegisterIndex(instruction, 3);

	machine_state->reg[r1] = machine_state->reg[r2] - machine_state->reg[r3];
}

void subi_function(uint32_t instruction, state *machine_state){
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);

	machine_state->reg[r1] = machine_state->reg[r2] - immediate;
}

void mul_function(uint32_t instruction, state *machine_state){
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	uint8_t r3 = extractRegisterIndex(instruction, 3);

	machine_state->reg[r1] = machine_state->reg[r2] * machine_state->reg[r3];
}

void muli_function(uint32_t instruction, state *machine_state){
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	machine_state->reg[r1] = machine_state->reg[r2] * immediate;
}

void lw_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	uint32_t result = machine_state->mem[r2 + immediate];
	machine_state->reg[r1] = result;
}

void sw_function(uint32_t instruction, state *machine_state) {
 	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	 
	machine_state->mem[r2 + immediate] = machine_state->reg[r1];
}

void beq_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 == r2) incrementPC(machine_state);
}

void bne_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 != r2) incrementPC(machine_state);
}

void blt_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 < r2) incrementPC(machine_state);
}

void bgt_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 > r2) incrementPC(machine_state);
}

void ble_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 <= r2) incrementPC(machine_state);
}

void bge_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	uint8_t r2 = extractRegisterIndex(instruction, 2);
	
	if (r1 >= r2) incrementPC(machine_state);
}

void jmp_function(uint32_t instruction, state *machine_state) {
	int32_t address = extractAddress(instruction);

	machine_state->pc = address;	
}

void jr_function(uint32_t instruction, state *machine_state) {
	uint8_t r1 = extractRegisterIndex(instruction, 1);
	machine_state->pc = machine_state->reg[r1];
}

void jal_function(uint32_t instruction, state *machine_state) {
	//To Do!
}

void out_function(uint32_t instruction, state *machine_state) {
 	//To Do!
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
