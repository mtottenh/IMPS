typedef struct Operands {
	uint32_t opcode;
	uint32_t operand1;
	uint32_t operand2;
	uint32_t operand3;
};

typedef void (*FunctionPointer)(Tokeniser_Line*);

void pass2(FILE*, Symbol_Table table);
int16_t eval_immediate(char*);
uint8_t eval_register(char*);

uint32_t assemble_halt(Tokeniser_Line*);
uint32_t assemble_rtype(Tokeniser_Line*);
uint32_t assemble_itype(Tokeniser_Line*);
uint32_t assemble_jtype(Tokeniser_Line*);
uint32_t assemble_fill(Tokeniser_Line*);
uint32_t assemble_skip(Tokeniser_Line*);
