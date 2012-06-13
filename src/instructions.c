#include "instructions.h"
#include "utils.h"

/* Increments the program counter by a given number of steps. */
void increment_pc(State *machine_state, int16_t i) {
	machine_state->pc = machine_state->pc + (4 * i);
}



/* Functions corresponding to the IMPS opcode functions. */
void halt_instruction(uint32_t instruction, State *machine_state) {
        /* Print the values of PC and registers, then terminate the program. */
        fprintf(stderr, " PC: 0x%x\t", machine_state->pc);

	uint8_t num_cols = 1;

        for(int i = 0; i < NUM_REGS; i++) {
                fprintf(stderr, "R%02d: 0x%x", i, machine_state->reg[i]);
		num_cols++;
		
		if (num_cols >= 4) {
			fprintf(stderr, "\n");
			num_cols = 0;
		}
		else {
			fprintf(stderr, "\t");
		} 
        }

	fprintf(stderr, "\n");
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
        uint32_t r2 = machine_state->reg[operands.r2];

	/* Check if memory access will be valid. If not, terminate. */
	int location = r2 + operands.immediate;
	if(check_mem_access(location)) {
		fprintf(stdout, "*** Terminating...\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Look at memory location as a pointer to uint32_t in order to access
	 * adjacent memory locations.
	 */
	uint32_t *result = (uint32_t *)&machine_state->mem[location];
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

	/* Check if memory access will be valid. If not, terminate. */
	int location = r2 + operands.immediate;
	if(check_mem_access(location)) {
		fprintf(stdout, "*** Terminating...\n");
		exit(EXIT_FAILURE);
	}

	/*
	 * Look at memory location as a pointer to uint32_t in order to access
	 * adjacent memory locations.
	 */
        uint32_t *pointer = (uint32_t *)&machine_state->mem[location];
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
        if (machine_state->reg[operands.r1] == machine_state->reg[operands.r2]) {
                increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void bne_instruction(uint32_t instruction, State *machine_state) {
        /*
         * If the values of two register operands are not equal, increment the
         * program counter by 4 * C, where C is an immediate value.
         * Otherwise, increment the program counter normally.
  	 */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] != machine_state->reg[operands.r2]) {
                increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void blt_instruction(uint32_t instruction, State *machine_state) {
        /*
         * If the value of the first register operand is less than that of
         * the second operand, increment the program counter by 4 * C, where C
         * is an immediate value.
         * Otherwise, increment the program counter normally.
         */
	OperandsI operands = extract_i(instruction);
	if (machine_state->reg[operands.r1] < machine_state->reg[operands.r2]) {
		increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void bgt_instruction(uint32_t instruction, State *machine_state) {
        /*
         * If the value of the first register operand is greater than that of
         * the second operand, increment the program counter by 4 * C, where C
         * is an immediate value.
         * Otherwise, increment the program counter normally.
         */
        OperandsI operands = extract_i(instruction);
        if (machine_state->reg[operands.r1] > machine_state->reg[operands.r2]) {
                increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void ble_instruction(uint32_t instruction, State *machine_state) {
        /*
         * If the value of the first register operand is less than or equal to
         * that of second operand, increment the program counter by 4 * C, where
         * C is an immediate value.
         * Otherwise, increment the program counter normally.
         */
        OperandsI operands = extract_i(instruction);
        if (machine_state->reg[operands.r1] <= machine_state->reg[operands.r2]) {
                increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void bge_instruction(uint32_t instruction, State *machine_state) {
        /*
         * If the value of the first register operand is grater than or equal to
         * that of second operand, increment the program counter by 4 * C, where
         * C is an immediate value.
         * Otherwise, increment the program counter normally.
         */
        OperandsI operands = extract_i(instruction);
        if (machine_state->reg[operands.r1] >= machine_state->reg[operands.r2]) {
                increment_pc(machine_state, operands.immediate);
	}
        else {
                increment_pc(machine_state,1);
	}
}

void jmp_instruction(uint32_t instruction, State *machine_state) {
        /* Jump to an address by setting the PC to a given operand. */
        uint32_t address = extract_address(instruction);

	/* Check whether the address is valid. If not, terminate. */
	if(check_address(address)) {
		fprintf(stdout, "*** Terminating..\n");
		exit(EXIT_FAILURE);
	}
	
        machine_state->pc = address;
}

void jr_instruction(uint32_t instruction, State *machine_state) {
        /*
         * Jump to an address by setting the PC to the value of a given
         * register operand.
         */
        OperandsR operands = extract_r(instruction);
        machine_state->pc = (uint16_t)machine_state->reg[operands.r1];
}

void jal_instruction(uint32_t instruction, State *machine_state) {
        /*
         * Store the instruction that would normally be executed next (PC + 4)
         * in R31 and set PC to a given operand.
         */	
        uint32_t address = extract_address(instruction);

	/* Check whether the address is valid. If not, terminate. */
	if(check_address(address)) {
		fprintf(stdout, "*** Terminating...\n");
		exit(EXIT_FAILURE);
	}

        machine_state->reg[31] = (machine_state->pc + 4);
        machine_state->pc = address;
}

void out_instruction(uint32_t instruction, State *machine_state) {
	/* Prints the least significant eight bits of R1 to stdout. */  
	OperandsR operands = extract_r(instruction);
	uint32_t regVal = machine_state->reg[operands.r1];
	uint32_t out = extract(regVal, END_INSTRUCTION - 7, END_INSTRUCTION);

	printf("%c", out);

	increment_pc(machine_state, 1);
} 

static void stack_dump(State* machine_state) {
	for (uint32_t i = MEM_SIZE -1; i > machine_state->sp; i-=4) {
		uint32_t *stackelem = (uint32_t *)&machine_state->mem[i];
		fprintf(stderr,"Stack Address [%d] : %d\t", (i - MEM_SIZE),*stackelem); 
		if (i % 2 == 0) 
			fprintf(stderr,"\n");
	}
}
/* S-Type instructions: 
 * things to think about, if the stack grows upward and we store at word as 4
 * bytes, when we retrive the word do we retrive the right way around?
 */
void push_instruction(uint32_t instruction, State *machine_state) {
	/* Check that there is enough room on the stack to perform an operation */
	if (machine_state->sp == machine_state->stack_boundary) {
		fprintf(stderr,"Error: Stack full, Terminating\n");
		stack_dump(machine_state);
		halt_instruction(0,machine_state);
		exit(EXIT_FAILURE);		
	}
	uint8_t flag = extract(instruction,6,7);
	uint32_t value = extract(instruction,8,31);
	uint32_t *result = (uint32_t *)&machine_state->mem[machine_state->sp];
	uint32_t *pointer = NULL;
	fprintf(stderr,"Flag: %x\tValue:%u\n",flag,value);
	switch(flag) {

		case 0:
		{
			*result = machine_state->reg[value];
			break;
		}
		case 1:
		{
			*result = value;
			break;
		}
		case 2:
		{
			uint16_t address = machine_state->reg[value];
			pointer = (uint32_t *)&machine_state->mem[address];
			
			*result = *pointer;
			break;
		}
		case 3:
		{
			pointer = (uint32_t *)&machine_state->mem[value];
				
			*result =*pointer;
			break;
		}
	}
	machine_state->sp -= 4;
	increment_pc(machine_state,1);
}

void pop_instruction(uint32_t instruction, State *machine_state) {
	/* check for empty stack if so throw error and halt */
	/*
	 * else extract last elem on stack and put it into register specified
	 * by #vlaue
	 */
	uint32_t reg = extract(instruction,8,31);
	machine_state->sp += 4;
	uint32_t *pointer = (uint32_t *)&machine_state->mem[machine_state->sp];
	machine_state->reg[reg] = *pointer;
	//machine_state->sp += 4;
	increment_pc(machine_state,1);
	fprintf(stderr, "Pc : %u, Value at sp : %u, sp : %u\n", machine_state->pc, *pointer,
                                                        machine_state->sp);

}

void call_instruction(uint32_t instruction, State *machine_state) {
	increment_pc(machine_state,1);
	/* Store jump location in value */
	uint32_t value = (uint32_t) extract(instruction,8,31);
	/* Point to a 32 bit word of memory starting at mem[SP] */
	uint32_t *pointer = (uint32_t *)&machine_state->mem[machine_state->sp];
	/* Set that 32 bit word to be PC */
	*pointer = (uint32_t) machine_state->pc;
	/* Decrease sp (stack grows downwards) */
	machine_state->sp -= 4;
	/* Jump to jump location by setting PC = value */
	machine_state->pc = value;
	fprintf(stderr, "Pc : %u, Value at sp : %u, sp : %u\n", machine_state->pc, *pointer, 
							machine_state->sp);
}
void ret_instruction(uint32_t instruction, State *machine_state) {
	/* As SP always points to the next free location
	 * increase it to get the last added element 
	 */
	machine_state->sp += 4;
	/* Set pointer to look at the last 32 bit word added to mem[sp] */
	uint32_t *pointer = (uint32_t *)&machine_state->mem[machine_state->sp];
	
	machine_state->pc = (uint16_t) *pointer;
	fprintf(stderr, "PC : %u SP: %u\n", machine_state->pc, machine_state->sp);
	
}
void mov_instruction(uint32_t instruction, State *machine_state) {
//	 moves 
	OperandsI operands = extract_i(instruction);
	machine_state->reg[operands.r1] = operands.immediate;
	increment_pc(machine_state,1);
}
void inc_instruction(uint32_t instruction, State *machine_state) {
	OperandsR operands = extract_r(instruction);
	machine_state->reg[operands.r1] += 1;
	increment_pc(machine_state,1);
}
