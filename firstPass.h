#ifndef FIRSTPASS_H
#define FIRSTPASS_H
#include "globals.h"
#include "dataCodeImages.h"

/* Processes a single line in the first pass (comment/guidance/instruction..), returns Whether succeeded */
bool readLineFirstPass(line line, int *IC, int *DC, codeImage** codeImg,struct symbolNode** symbolTable,struct dataImage** dataImg);

#endif
