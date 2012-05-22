nclude <stdio.h>
#include <stdint.h>
#include <string.h>

#define MEM_SIZE 16384
#define NUM_REGS 32


struct state {
	uint8_t *pc;
	uint32_t mem[MEM_SIZE];
	uint32_t reg[NUM_REGS];
};

int init(struct state*);

int main(int argc, char **argv) {
	struct state current;

	init(&current);

	return 0;
}

/* initialises a machine state*/
int init(struct state *machine_state) {
	/*
	 * Sets PC to first memory location
	 * PC is a pointer to 8 bits of memory;
	 * has to be byte addressable
	 */
	 (*machine_state).pc = (uint8_t *)(*machine_state).mem;

	 /* Initialise memory to 0 - see man 3 memset */
	 memset((*machine_state).mem, 0, sizeof(uint32_t)*MEM_SIZE);
	 memset((*machine_state).reg, 0, sizeof(uint32_t)*NUM_REGS);
	 return 0;
}


/*useful code
 * for(int i = 0; i<MEM_SIZE; i++) {
		printf("current.mem[%d]  : %d\n", i , current.mem[i]);
	}*/

