/* Helper functions to process and analyze code */
#ifndef _GUIDANCES_H
#define __GUIDANCES_H
#include "globals.h"
#include "dataCodeImages.h"

void is_guidance(char *name_guide, guide *get_guide);
guide guidanceByIndex(line line, int *i);
guide guidanceByName(char *name);
bool guideHandling(guide guidance,struct symbolNode** symbolTable, char* label, line line,int *DC,int i,dataImage** dataImg,bool SymbolFlag);
bool DbDhDwguide(line line, int i,dataImage** dataImg, int *DC,int bytes);
bool Ascizguide(line line, int i,dataImage** dataImg, int *DC);
#endif

