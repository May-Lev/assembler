
#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H
#include "globals.h"
#include "instructions.h"
#include "dataCodeImages.h"

/*bool RIJInstruct(line line, int i,codeImage** codeImg, int *IC,char RIJ);*/
bool instructHandling(char* instructName, line line,int *IC,int i,codeImage** codeImg,opcode *op, funct *fun, char ** operands);
bool cheakLongReg(char **operands,int j);
#endif
