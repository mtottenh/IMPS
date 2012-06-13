#ifndef _UTILS_H_
#define _UTILS_H_
#include <stdio.h>
#include <stdint.h>

#define END_INSTRUCTION  31
#define MEM_SIZE 65536
#define NUM_REGS 32
#define NUM_OPCODES 26

/* Address format constants. */
static uint8_t const START_OPCODE = 0;
static uint8_t const END_OPCODE = 5;
static uint8_t const REG_WIDTH = 5;

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

uint32_t extract(uint32_t, uint8_t, uint8_t);
uint8_t extract_opcode(uint32_t);
uint32_t extract_address(uint32_t);

OperandsR extract_r(uint32_t);
OperandsI extract_i(uint32_t);

int is_valid_opcode(uint8_t);
int check_mem_access(int);
int check_address(uint32_t);

#endif

