#ifndef SECONDPASS_H
#define SECONDPASS_H
#include "globals.h"
#include "dataCodeImages.h"
bool readLineSecondPass(line line, int *IC, codeImage** codeImg,struct symbolNode** symbolTable, externList** externalList);
#endif
