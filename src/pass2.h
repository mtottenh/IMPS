typedef struct Operands {
	uint32_t opcode;
	uint32_t operand1;
	uint32_t operand2;
	uint32_t operand3;
	
};

typedef void (*FunctionPointer)(uint32_t, Tokeniser_Line*, Symbol_Table);

void pass2(FILE*, FILE*, Symbol_Table*);
uint32_t eval_immediate(char*, uint32_t, Symbol_Table*);
uint32_t eval_register(char*);

uint32_t assemble_halt(Tokeniser_Line*);
uint32_t assemble_rtype(Tokeniser_Line*);
uint32_t assemble_itype(Tokeniser_Line*);
uint32_t assemble_jtype(Tokeniser_Line*);
uint32_t assemble_fill(Tokeniser_Line*);
