
#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H
#include "globals.h"
#include "instructions.h"
#include "dataCodeImages.h"


bool instructHandling(char* instructName, line line,int IC,int i,codeImage** codeImg,opcode *op, funct *fun);
#endif
