#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "firstPass.h"
#include "helpFuncs.h"
#include "opcode.h"
#include "guidances.h"
#include "instructions.h"
#include "dataCodeImages.h"
#include "symbolTable.h"
bool readLineSecondPass(line line, int *IC, codeImage** codeImg,struct symbolNode** symbolTable, externList** externalList)
{
	int i = 0;
	char *labelEntr;
	char * str;
	struct symbolNode *nodeSymbol = NULL;
	SKIP_WHITE_SPACE(line.text , i)
    if(isComment(line , i))
		return TRUE;	
	SKIP_WHITE_SPACE(line.text , i)
	if (line.text[i] == '.')
	{
		if (strncmp(".entry", line.text, 6) == 0)
		{
			i+=6;
			SKIP_WHITE_SPACE(line.text , i)
			labelEntr = strtok(line.text + i, " \n\t");
			if (labelEntr == NULL)
				 return printError(line.number, "No name for label in entry guidance.");
			labelEntr = strtok(line.text + i, "\n"); /*get name of label*/
			nodeSymbol = isSymbolInTable(symbolTable,labelEntr);
			if(nodeSymbol == NULL)
				return printError(line.number, "The label is not in the symbol table.");
			if(isExternSymbol(labelEntr,symbolTable))
				return printError(line.number, "The symbol %s can be either external or entry, but not both.",
						                  labelEntr);
			str = malloc(strlen(nodeSymbol->attribute) + strlen(", entry") + 1);
    		strcpy(str, nodeSymbol->attribute);
    		strcat(str, ", entry");
			nodeSymbol->attribute = str;

		}

	}
	else /*instruct*/
	{
			int j, op , fun,operandCount = 0, adressSymbol,adressExtern;
			char* operands[MAX_OPERANDS_INST];
			char instructName[INST_MAX_LEN];
			for (j = 0; line.text[i] && line.text[i] != '\t' && line.text[i] != ' ' &&
			 line.text[i] != '\n' && line.text[i] != EOF && j < (INST_MAX_LEN+1); i++, j++)
			{
				instructName[j] = line.text[i];
			}
			instructName[j] = '\0';
			is_opcode(instructName, &op ,(funct*)&fun);
			SKIP_WHITE_SPACE(line.text , i)
			operandsCheck(line, i, operands, &operandCount, instructName);

			if(op>=15 && op<=18)
			{
				if(isExternSymbol(operands[2],symbolTable))
					return printError(line.number, "The label cannot be external in Conditional branching.");
				nodeSymbol = isSymbolInTable(symbolTable,operands[2]);
					if(nodeSymbol == NULL)
						return printError(line.number, "The label is not in the symbol table.");
				adressSymbol = getAdressForLable(operands[2],symbolTable);
				adressSymbol = adressSymbol-*IC;
				updateBinCodeImg(codeImg,*IC, adressSymbol,'I');
			}
			else if(op>=30 && op<=32) /*j*/
			{
				operands[0] = strtok(line.text + i, " \n\t");
				if(operands[0][0]=='$')	
					return TRUE;
				adressSymbol = getAdressForLable(operands[0],symbolTable);
				if(isExternSymbol(operands[0],symbolTable))
				{
					adressExtern = getAdressCodeImg(codeImg,line.text,*IC);
					addExternLabel(externalList,operands[0],adressExtern);
					updateBinCodeImg(codeImg,*IC, 0,'J');
					return TRUE;
				}
				nodeSymbol = isSymbolInTable(symbolTable,operands[0]);
					if(nodeSymbol == NULL)
					{
						return printError(line.number, "The label is not in the symbol table.");
					}
				adressSymbol = getAdressForLable(operands[0],symbolTable);
				updateBinCodeImg(codeImg,*IC, adressSymbol,'J');
			}
			return TRUE;
	}
			/*printExternalList(*externalList);*/
			/*printSymbolTable(*symbolTable);*/

	return TRUE;	
}
