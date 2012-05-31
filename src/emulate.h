#ifndef _EMULATE_H_
#define _EMULATE_H_

#include "instructions.h"
#include "utils.h"
#include <string.h>

/* State initialisation and pointer setup functions. */
int init(State*);
void setup_pointers(FunctionPointer[]);

#endif
