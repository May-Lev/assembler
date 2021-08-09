/* Helper functions to process and analyze code */
#ifndef _INSTRUCTIONS_H
#define _INSTRUCTIONS_H
#include "globals.h"

/**
 *void is_instructions(char *name_ins)
 * @param name_ins The command name (string)

 */
void is_instruction(char *name_ins, instruct *get_ins);
instruct instructionByIndex(line line, int *i);
instruct instructionByName(char *name);
bool instructHandling(instruct instruction,struct symbolNode** symbolTable, char* lable,line line,int DC,int i,int dataImg);
bool DbDhDwinstruct(line line, int i,/*struct symbolNode***/ int dataImg, int DC,int bytes);
#endif

