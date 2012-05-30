#ifndef _UTILS_H_
#define _UTILS_H_

#define END_INSTRUCTION = 31;
/* Type of function pointer to opcode functions. */
typedef void (*FunctionPointer)(uint32_t, State*);

/* State initialisation and extraction functions. */
int init(State*);

uint32_t extract(uint32_t,uint8_t,uint8_t);
uint8_t extract_opcode(uint32_t);
uint32_t extract_address(uint32_t);

void setup_pointers(FunctionPointer array[]);
void increment_pc(State*, int16_t);
/*
 * Given a bit pattern corresponding to an R-Type instruction, this function
 * returns a struct containing the R1, R2 and R3 components of the instruction.
 */
OperandsR extract_r(uint32_t instruction);
/*
 * Given a bit pattern corresponding to an I-Type instruction, this function
 * returns a struct containing the R1, R2 and intermediate value components
 * of the instruction.
 */
OperandsI extract_i(uint32_t instruction);

#endif

