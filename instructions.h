#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H
#include "globals.h"
#include "dataCodeImages.h"

/* The function gets an instruction and process it according to the type of instruction. Returns false if failed/ wrong format of instruction. +addToCodeImg */
bool instructHandling(char* instructName, line line,int *IC,int i,codeImage** codeImg,opcode *op, funct *fun, char ** operands,struct symbolNode** symbolTable);
/* checks registers with two digits name (for example, 31$) */
bool cheakLongReg(char **operands,int j);
#endif
