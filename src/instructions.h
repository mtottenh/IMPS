#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include "utils.h"


/* Structure for the emulators state. */
typedef struct State {
        uint16_t pc;
        uint8_t mem[MEM_SIZE];
        uint32_t reg[NUM_REGS];
} State;
/* Type of function pointer to opcode functions. */
typedef void (*FunctionPointer)(uint32_t, State*);


/* State initialisation and extraction functions. */
void increment_pc(State*, int16_t);
void setup_pointers(FunctionPointer array[]);

/* Functions corresponding to the IMPS opcode functions. */
void halt_instruction(uint32_t, State*);
void add_instruction(uint32_t, State*);
void addi_instruction(uint32_t, State*);
void sub_instruction(uint32_t, State*);
void subi_instruction(uint32_t, State*);
void mul_instruction(uint32_t, State*);
void muli_instruction(uint32_t, State*);
void lw_instruction(uint32_t, State*);
void sw_instruction(uint32_t, State*);
void beq_instruction(uint32_t, State*);
void bne_instruction(uint32_t, State*);
void blt_instruction(uint32_t, State*);
void bgt_instruction(uint32_t, State*);
void ble_instruction(uint32_t, State*);
void bge_instruction(uint32_t, State*);
void jmp_instruction(uint32_t, State*);
void jr_instruction(uint32_t, State*);
void jal_instruction(uint32_t, State*);
void out_instruction(uint32_t, State*); 

#endif
