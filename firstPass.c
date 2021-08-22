#include <stdio.h>
#include <stdlib.h>
#include "firstPass.h"
#include "helpFuncs.h"
#include "opcode.h"
#include "guidances.h"
#include "instructions.h"
#include "dataCodeImages.h"
#include "symbolTable.h"

bool SymbolFlag = FALSE;
bool readLineFirstPass(line line, int *IC, int *DC, codeImage** codeImg,struct symbolNode** symbolTable,dataImage** dataImg)
{
	int i = 0;
	char label[MAX_LINE];	
	guide guidance;
	SymbolFlag = FALSE;
	SKIP_WHITE_SPACE(line.text , i)
	/* Empty line or a comment, skip*/
   	 if(isComment(line , i))
		return TRUE;
	/* Checks and updates the label */
    if(is_label(line,label))
	    return FALSE;
	if (label[0] && !valid_label(label))
		return printError(line.number,"Illegal label name: %s", label);
	if (label[0] != '\0')
	{
	/* Raise a flag, symbol was found */
		SymbolFlag = TRUE;
	/* Continue with text after the symbol*/							         
		for (; line.text[i] != ':'; i++); 
			i++;
	}
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == '\n')
		return TRUE;
	/* Checks if there is a guidance (starts with '.') */
	guidance = guidanceByIndex(line, &i);
	SKIP_WHITE_SPACE(line.text, i)
	if(guidance != NONE_GUIDE) /* It's a guidance */  	  
	{	
		if(SymbolFlag && guidance != EXTERN_GUIDE)
			addSymbolTable(symbolTable, label, *DC, "data");
		return (guideHandling(guidance,symbolTable,label,line,DC,i,dataImg,SymbolFlag));
	}
	else	 /* It's an instruction */
	{		
		int op , fun, j,operandCount = 0;
		char instructName[INST_MAX_LEN];
		char* operands[MAX_OPERANDS_INST];
		if(SymbolFlag)
			addSymbolTable(symbolTable, label, *IC, "code");
		/* Making a string of the instruction name */
		for (j = 0; line.text[i] && line.text[i] != '\t' && line.text[i] != ' ' &&
			 line.text[i] != '\n' && line.text[i] != EOF && j < (INST_MAX_LEN+1); i++, j++)
		{
			instructName[j] = line.text[i];
		}
		instructName[j] = '\0';
		/* Gets the opcode and funct for instructHandling */
		is_opcode(instructName, &op ,(funct*)&fun);
		if (!operandsCheck(line, i, operands, &operandCount, instructName))
			return FALSE;
		return(instructHandling(instructName,line,IC,i,codeImg,&op ,(funct*)&fun,operands,symbolTable));
	}
}
