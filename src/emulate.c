#include "emulate.h"

int main(int argc, char **argv) {

	/*
	 * Allocate memory for our machine state. Terminate in the event of
	 * failure.
	 */

	State* current = malloc(sizeof(State));
	if (current == NULL) {
		perror("*** Couldn't allocate memory to machine state. Terminating");
		return EXIT_FAILURE;
	}	

	/* Initialise our state and setup opcode function pointers. */
	init(current);
	FunctionPointer func_pointers[27] = {NULL};
	setup_pointers(func_pointers);

	FILE *binary_file;

	/* Get binary file path. */
	char *file_path = argv[1];

	/* Open binary file. */
	if((binary_file = fopen(file_path, "rb")) == NULL) {
		perror("*** Couldn't open file specified at runtime. Terminating");
		return EXIT_FAILURE;
	}

	/* Read binary file into the current state's memory */
	fread(current->mem, sizeof(uint8_t), MEM_SIZE, binary_file);
	fclose(binary_file);

	freopen(NULL, "wb", stdout);

	/* Begin decode execute loop */
	uint8_t opcode;
	fprintf(stderr,"***Machine's Intial State***\n");
	fprintf(stderr,"PC: %d\t SP: %d\t, StackSize: %d\n", current->pc,current->sp, current->stack_size);
	do {
		uint32_t *instruction = (uint32_t *)&current->mem[current->pc];
		
		opcode = extract_opcode(*instruction);
		fprintf(stderr,"Opcode : %d\t, Instruction : %x\n", opcode, *instruction);

		if (is_valid_opcode(opcode)) {
			func_pointers[opcode](*instruction, current);
		}

		else {
			increment_pc(current, 1);
		}
	} while (opcode != 0);

	/* Deallocate the memory assigned to the machine state. */
	free(current);

	return EXIT_SUCCESS;
}

/* Initialises a machine state */
int init(State *machine_state) {
	/* Sets PC to first memory location.*/
	machine_state->pc = 0;
	/*
         * Initialise the stack pointer to 4 bytes from the top of memory 
	 * NB: MEM_SIZE is top of memory + 1 due to 0 based arrays
         */
	machine_state->sp = MEM_SIZE - 5;
	/* set defualt stack size to 1024 bytes */
	machine_state->stack_size = 1024;
	machine_state->stack_boundary = machine_state->sp - machine_state->stack_size;
	/* Initialise memory to 0. */
	memset(machine_state->mem, 0, sizeof(uint8_t) * MEM_SIZE);
	memset(machine_state->reg, 0, sizeof(uint32_t) * NUM_REGS);
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
	/* opcodes 19/20 are assembler directives and are not used */
	array[19] = NULL;
	array[20] = NULL;
	/* opcodes 21 -> 24 are S-Type instructions (stack related) */
	array[21] = &push_instruction;
	array[22] = &pop_instruction;
	array[23] = &call_instruction;
	array[24] = &ret_instruction;
	array[25] = &mov_instruction;
	array[26] = &inc_instruction;
}


