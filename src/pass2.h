#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "tokeniser.h"

typedef uint32_t (*FunctionPointer)(uint32_t, Tokeniser_Line, Symbol_Table*);

void pass2(FILE*, FILE*, Symbol_Table*);
uint32_t eval_immediate(char*, uint32_t, Symbol_Table*);
uint32_t eval_register(char*);
void setup_pointers(FunctionPointer[]);

uint32_t assemble_halt(uint32_t, Tokeniser_Line*, Symbol_Table*);
uint32_t assemble_rtype(uint32_t, Tokeniser_Line*, Symbol_Table*);
uint32_t assemble_itype(uint32_t, Tokeniser_Line*, Symbol_Table*);
uint32_t assemble_jtype(uint32_t, Tokeniser_Line*, Symbol_Table*);
uint32_t assemble_fill(uint32_t, Tokeniser_Line*, Symbol_Table*);
