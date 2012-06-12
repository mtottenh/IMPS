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
#define INSTR_WIDTH 32
#define OPCODE_WIDTH 6
#define REG_WIDTH 5

#define STYPE_FLAGS_WIDTH 2
#define STYPE_IMMEDIATE_FLAG 0x1
#define STYPE_MEM_FLAG 0x2

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
uint32_t eval_immediate(char*, Symbol_Table*);
uint32_t eval_register(char*);
uint32_t eval_stype(Symbol_Table*, char*, uint32_t*);
void setup_pointers(FunctionPointer[]);

uint32_t assemble_halt(Instruction);
uint32_t assemble_rtype(Instruction);
uint32_t assemble_itype(Instruction);
uint32_t assemble_branch(Instruction);
uint32_t assemble_jtype(Instruction);
uint32_t assemble_stype(Instruction);
uint32_t assemble_fill(Instruction);
uint32_t assemble_skip(Instruction);

#endif
