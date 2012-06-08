#ifndef _PASS2_H_
#define _PASS2_H_

#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "tokeniser.h"

#define MEM_SIZE 65536
#define START_BRANCH 9 
#define END_BRANCH 14
#define SKIP_OPCODE 20

typedef struct Instruction {
	uint32_t opcode;
	char* operand1;
	char* operand2;
	char* operand3;
	Symbol_Table* table;
	uint16_t* address;
} Instruction;

typedef uint32_t (*FunctionPointer)(Instruction);

void pass2(FILE*, FILE*, Symbol_Table*);
uint32_t eval_immediate(char*, uint32_t, Symbol_Table*);
uint32_t eval_register(char*);
void setup_pointers(FunctionPointer[]);

uint32_t assemble_halt(Instruction);
uint32_t assemble_rtype(Instruction);
uint32_t assemble_itype(Instruction);
uint32_t assemble_branch(Instruction);
uint32_t assemble_jtype(Instruction);
uint32_t assemble_fill(Instruction);
uint32_t assemble_skip(Instruction);

#endif
