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
	/*Read contense of binary file into state*/
		/*Read binary file 32 *BITS* At a time*/
		/*Convert the 32 bits/chars into a uint32_t*/
		/*store each uint32_t into state.mem*/
	/*Start decode execute loop*/
		/*Finish if halt encountered*/
	return 0;
}
/*extract opcode*/
#define START_OPCODE 0
#define END_OPCODE 5
uint8_t extract_opcode(uint32_t instruction) {
	
	return extract(instruction,START_OPCODE, END_OPCODE);
}
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
