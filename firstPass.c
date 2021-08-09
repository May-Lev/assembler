#include <stdio.h>
#include <stdlib.h>
#include "firstPass.h"
#include "helpFuncs.h"
#include "opcode.h"
#include "instructions.h"
bool SymbolFlag = FALSE;
bool readLine(line line, int *IC, int *DC, int code_img,struct symbolNode** data_img, int symbol_table)
{
	int i =0;
	char label[MAX_LINE];	
	instruct instruction;
	SymbolFlag = FALSE;
	
	SKIP_WHITE_SPACE(line.text , i)
    if(isComment(line , i))/*empty orv comment line*/
	{
			/*printf("comment\n");*/
		return TRUE;
	}
    if(is_label(line,label))
	    return FALSE;
	if (label[0] && !valid_label(label))
	{
		printError(line.number,"Illegal label name: %s", label);
		return FALSE;
	}
	if (label[0] != '\0')
	{
		SymbolFlag = TRUE;
		/*printf("label ");*/
		for (; line.text[i] != ':'; i++); /* if symbol detected, start analyzing from it's deceleration end */
		i++;
	}
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == '\n')
		return TRUE; /* Label-only line - skip */
	/* Check if it's an instruction (starting with '.') */
	instruction = instructionByIndex(line, &i);
	SKIP_WHITE_SPACE(line.text, i)
	if(instruction == NONE_INS)/* or in the instructHandling func*/
	{
		/*int op , fun;*/
		if(SymbolFlag)
			addSymbolToDataImg(data_img, label, *IC, "code");
		/*is_opcode(line.text, &op ,(funct*)&fun);*//**/
		/*printf("op %d\n", op);
		printf("fun %d\n", fun);
		printf("horaa\n");*/
	}
	else
	{
		/*if(SymbolFlag)חוזר על עצמו?instructHandling כי ב פונקציה גם מכניסים לטבלה
			addSymbolToDataImg(data_img, label, *DC, "data");*/
		/*printf("instruction\n");*/
		if(!instructHandling(instruction,data_img,label,line,*DC/*גם ic?*/,i))/*return false*/
			return FALSE;
	}
	
	return TRUE; /**/
}
