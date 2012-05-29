#define MEM_SIZE 16384
#define NUM_REGS 32

// Structure for the emulators state
typedef struct state {
	uint8_t *pc;
	uint32_t mem[MEM_SIZE];
	uint32_t reg[NUM_REGS];
} state;

// Structure that represent the operands of R and I type instructions
typedef struct operandsR {
	uint8_t r1;
	uint8_t r2;
	uint8_t r3;
} operandsR;

typedef struct operandsI {
	uint8_t r1;
	uint8_t r2;
	int16_t immediate;
} operandsI;

// Type of function pointer to opcode functions
typedef void (*functionPointer)(uint32_t, state*);

// State initialisation and extraction functions 
int init(state*);
uint32_t extract(uint32_t,uint8_t,uint8_t);
uint8_t extract_opcode(uint32_t);
uint32_t extract_address(uint32_t);

void setup_pointers(functionPointer array[]);

void increment_pc(state*);

// Prototypes for opcode instructions
void halt_instruction(uint32_t, state *);
void add_instruction(uint32_t, state *);
void addi_instruction(uint32_t, state *);	
void sub_instruction(uint32_t, state *);
void subi_instruction(uint32_t, state *);
void mul_instruction(uint32_t, state *);
void muli_instruction(uint32_t, state *);
void lw_instruction(uint32_t, state *);
void sw_instruction(uint32_t, state *);
void beq_instruction(uint32_t, state *);
void bne_instruction(uint32_t, state *);
void blt_instruction(uint32_t, state *);
void bgt_instruction(uint32_t, state *);
void ble_instruction(uint32_t, state *);
void bge_instruction(uint32_t, state *);
void jmp_instruction(uint32_t, state *);
void jr_instruction(uint32_t, state *); 
void jal_instruction(uint32_t, state *);
void out_instruction(uint32_t, state *);
