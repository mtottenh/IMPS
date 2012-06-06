#include <stdio.h>
#include "symbol_table.h"


int main(void) {
	Symbol_Table* s = Symbol_Table_new();
	printf("\nLength of symbol Table is: %d\n", (int)Symbol_Table_getLength(s));

	printf("Opcode halt is:%d ", Symbol_Table_getPrev(s,"sub")->value);
	printf("Value of s: %p \tValue of head of list: %d\n",(void*)s,s->head->value);
	Symbol_Table_print(s);
	Symbol_Table_free(&s);
//	s = NULL;
	printf("Value of s: %p\n",(void*)s);
	printf("Length of Symbol Table after free: %d\n",(int)Symbol_Table_getLength(s));
	Symbol_Table_print(s);
	return 0;
}
