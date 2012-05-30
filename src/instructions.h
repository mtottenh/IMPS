#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_
#include <stdint.h>

#define MEM_SIZE 65536
#define NUM_REGS 32

/* Structure for the emulators state. */
typedef struct State {
        uint8_t *pc;
        uint8_t mem[MEM_SIZE];
        uint32_t reg[NUM_REGS];
} State;

/* Structure that represent the operands of R and I type instructions. */
typedef struct OperandsR {
        uint8_t r1;
        uint8_t r2;
        uint8_t r3;
} OperandsR;

typedef struct OperandsI {
        uint8_t r1;
        uint8_t r2;
        int16_t immediate;
} OperandsI;

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
