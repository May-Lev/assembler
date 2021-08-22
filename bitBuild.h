#ifndef _BIT_BUILD_H
#define _BIT_BUILD_H
#include "globals.h"


/* The functions creates and returns codeBin structure for R,I,J instructions*/
codeBinR * MakeRBin(opcode *op, funct *fun, char** operands);
codeBinI * MakeIBin(opcode *op, funct *fun, char** operands, int IC,void* symbolTable);
codeBinJ * MakeJBin(opcode *op, funct *fun, char** operands,void* symbolTable);
#endif
