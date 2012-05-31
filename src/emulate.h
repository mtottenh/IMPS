#ifndef _EMULATE_H_
#define _EMULATE_H_

#include "instructions.h"
#include "utils.h"


/* State initialisation and extraction functions. */
int init(State*);

void setup_pointers(FunctionPointer array[]);

#endif
