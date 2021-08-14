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
bool readLine(line line, int *IC, int *DC, codeImage** codeImg,struct symbolNode** symbolTable,dataImage** dataImg)
{
	int i = 0;
	char label[MAX_LINE];	
	guide guidance;
	SymbolFlag = FALSE;
	SKIP_WHITE_SPACE(line.text , i)
    if(isComment(line , i))
                            /*printf("empty or comment\n");*/
		return TRUE;
    if(is_label(line,label))
	    return FALSE;
	if (label[0] && !valid_label(label))
	{
		return printError(line.number,"Illegal label name: %s", label);
	}
	if (label[0] != '\0')
	{
		SymbolFlag = TRUE;
									         	/*printf("label ");*/
		for (; line.text[i] != ':'; i++); /* Continue with text after the symbol */
			i++;
	}
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == '\n')
		return TRUE;
		
	/* Check if it's an guidance (starting with '.') */
	guidance = guidanceByIndex(line, &i);
	SKIP_WHITE_SPACE(line.text, i)
	if(guidance != NONE_GUIDE)        	    /*printf("guidance\n");*/    
	{	
		if(SymbolFlag && guidance != EXTERN_GUIDE)
			addSymbolTable(symbolTable, label, *DC, "data");
		/*addToDataImg(DC,line.text);*/
		return (guideHandling(guidance,symbolTable,label,line,DC/*גם ic?*/,i,dataImg));
	}
	else
	{/* or in the guideHandling func*/
								 /* instructions */
		int op , fun, j,operandCount = 0;
		char instructName[INST_MAX_LEN];
		char* operands[MAX_OPERANDS_INST];
		if(SymbolFlag)/*in instructHandling?*/
			addSymbolTable(symbolTable, label, *IC, "code");
		/*making string instructName*/
		for (j = 0; line.text[i] && line.text[i] != '\t' && line.text[i] != ' ' &&
			 line.text[i] != '\n' && line.text[i] != EOF && j < (INST_MAX_LEN+1); i++, j++)
		{
			instructName[j] = line.text[i];
		}
		instructName[j] = '\0';
		is_opcode(instructName, &op ,(funct*)&fun);
		if (!operandsCheck(line, i, operands, &operandCount, instructName))
			return FALSE;
		/*addToCodeImg(IC,line.text);*/
		return(instructHandling(instructName,line,IC,i,codeImg,&op ,(funct*)&fun,operands));
	}
	
	return TRUE; /**/
}
