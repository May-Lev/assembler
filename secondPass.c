#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "firstPass.h"
#include "helpFuncs.h"
#include "opcode.h"
#include "symbolTable.h"

bool readLineSecondPass(line line, int *IC,int *icf, codeImage** codeImg,struct symbolNode** symbolTable, exorEnList** externList,exorEnList** enterList)
{
	int i = 0,adressExorEn;
	char *labelEntr;
	char * str;
	struct symbolNode *nodeSymbol = NULL;
	SKIP_WHITE_SPACE(line.text , i)
    if(isComment(line , i))
		return TRUE;	
	SKIP_WHITE_SPACE(line.text , i)
	if (line.text[i] == '.')
	{
		/* Checks an entry label of guidance */
		if (strncmp(".entry", line.text, 6) == 0)
		{
			i+=6;
			SKIP_WHITE_SPACE(line.text , i)
			labelEntr = strtok(line.text + i, " \n\t");
			if (labelEntr == NULL)
				 return printError(line.number, "No name for label in entry guidance.");
			labelEntr = strtok(line.text + i, "\n"); /* Get name of label */
			nodeSymbol = isSymbolInTable(symbolTable,labelEntr);
			if(nodeSymbol == NULL)
				return printError(line.number, "The label %s is not in the symbol table.",labelEntr);
			if(isExternSymbol(labelEntr,symbolTable))
				return printError(line.number, "The symbol %s can be either external or entry, but not both.",
						                  labelEntr);
			/* Add entry to the symbol table and entry list */
			str =check_malloc(strlen(nodeSymbol->attribute) + strlen(", entry") + 1);
    		strcpy(str, nodeSymbol->attribute);
    		strcat(str, ", entry");
			nodeSymbol->attribute = str;
			adressExorEn = nodeSymbol->value;
			addExorEntLabel(enterList,labelEntr,adressExorEn);
		}
	}
	else /*instruct*/
	{
		int j, op , fun,operandCount = 0, adressSymbol;
		char* operands[MAX_OPERANDS_INST];
		char instructName[INST_MAX_LEN];
		/* Making a string of the instruction name */
		for (j = 0; line.text[i] && line.text[i] != '\t' && line.text[i] != ' ' &&
		 line.text[i] != '\n' && line.text[i] != EOF && j < (INST_MAX_LEN+1); i++, j++)
		{
			instructName[j] = line.text[i];
		}
		instructName[j] = '\0';
		/* Gets the opcode */
		is_opcode(instructName, &op ,(funct*)&fun);
		SKIP_WHITE_SPACE(line.text , i)
		operandsCheck(line, i, operands, &operandCount, instructName);
		if(op>=BNE_OP && op<=BGT_OP) /* Conditional branching (I) */
		{
			if(isExternSymbol(operands[2],symbolTable))
				return printError(line.number, "The label cannot be external in Conditional branching.");
			nodeSymbol = isSymbolInTable(symbolTable,operands[2]);
				if(nodeSymbol == NULL)
					return printError(line.number, "The label %s is not in the symbol table.",operands[2]);
			adressSymbol = getAdressForLable(operands[2],symbolTable);
			adressSymbol = *IC-adressSymbol;
			/* Update the address of I instruction */
			updateBinCodeImg(codeImg,*IC, adressSymbol,'I',line.text);
		}
		else if(op>=JMP_OP && op<=CALL_OP) /* J */
		{
			operands[0] = strtok(line.text + i, " \n\t");
			if(operands[0][0]=='$')	/* Instruction using register, the address in  known */
				return TRUE;
			adressSymbol = getAdressForLable(operands[0],symbolTable);
			if(isExternSymbol(operands[0],symbolTable))
			{
				adressExorEn = getAdressCodeImg(codeImg,line.text,*icf);
				addExorEntLabel(externList,operands[0],adressExorEn);
				/* Update the address of an external label */
				updateBinCodeImg(codeImg,*IC, 0,'J',line.text);
				return TRUE;
			}
			nodeSymbol = isSymbolInTable(symbolTable,operands[0]);
			if(nodeSymbol == NULL)
				return printError(line.number, "The label %s is not in the symbol table.",operands[0]);
			/* Update the address of J instruction */
			updateBinCodeImg(codeImg,*IC, adressSymbol,'J',line.text);
		}
	}
	return TRUE;	
}
