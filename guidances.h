#ifndef _GUIDANCES_H
#define _GUIDANCES_H
#include "globals.h"
#include "dataCodeImages.h"

/* The function updates the guidence, if was found*/
void is_guidance(char *name_guide, guide *get_guide);
/* The function returns the guidence, if was found by name in guideFindTable*/
guide guidanceByIndex(line line, int *i);
/* The function gets a line and index from the text, and returns the guidance, if was found */
guide guidanceByName(char *name);
/* The function gets a guidance and passes it to different functions, according to the type of guidance. Returns false if failed/ wrong format of guidance. */
bool guideHandling(guide guidance,struct symbolNode** symbolTable, char* label, line line,int *DC,int i,dataImage** dataImg,bool SymbolFlag);
/* The function gets a DB/DH/DW guidance, checks and add it to the Data Image */
bool DbDhDwguide(line line, int i,dataImage** dataImg, int *DC,int bytes);
/* The function gets an Asciz guidance, checks and add it to the Data Image */
bool Ascizguide(line line, int i,dataImage** dataImg, int *DC);
#endif

