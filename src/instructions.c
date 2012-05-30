#include <stdio.h>
#include "instructions.h"
#include "utils.h"

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
         *      machine_state->pc = &machine_state->reg[operands.r1];
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

