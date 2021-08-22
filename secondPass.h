#ifndef SECONDPASS_H
#define SECONDPASS_H
#include "globals.h"
#include "dataCodeImages.h"
#include "symbolTable.h"

/* Processes a single line in the second pass (entry/external/instruction..), returns Whether succeeded */
bool readLineSecondPass(line line, int *IC,int *icf, codeImage** codeImg,struct symbolNode** symbolTable, exorEnList** externList,exorEnList** enterList);
#endif
