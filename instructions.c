#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "instructions.h"
#include "globals.h"
#include "bitBuild.h"
bool instructHandling(char* instructName, line line,int *IC,int i,codeImage** codeImg,opcode *op, funct *fun, char ** operands,struct symbolNode** symbolTable)
{
	/* The order and number of operands is different in different types of instructions */
	int numReg = 2,placeOfIm = 2,placeOfReg = 1,returnI;
	int j;
	/* Char of R/IJ instructions */
	char RIJ;
	codeBinR* binROut = NULL;
	codeBinI* binIOut = NULL;
	codeBinJ* binJOut = NULL;
	if(*op == NONE_OP)
		return printError(line.number,"Unrecognized instruction: %s", instructName);
	if(*op ==1|| *op==0) /* R instruction */
	{
		RIJ = 'R';
		if(*op==0)
			numReg = 3;
		for(j = 0; j<numReg; j++)
		{	
			if(!(*operands[j]))
				return printError(line.number,"Incorrect amount of operands.");
			if((operands[j][0]!= '$')||(operands[j][3])||!cheakLongReg(operands,j))
				return(printError(line.number,"Unrecognized register: %s", operands[j]));
		}
		binROut = MakeRBin(op,fun,operands);
	}
	else if(*op <=SH_OP && *op>=ADDI_OP) /* I instruction */
	{
		RIJ = 'I';
		if((*op <=NORI_OP && *op>=ADDI_OP) || (*op <=SH_OP && *op>=LB_OP))
		{
			placeOfIm = 1;
			placeOfReg = 2;
		}
		if(!(*operands[2]))
		{
			return printError(line.number,"Incorrect amount of operands.");
		}
		if((operands[0][0]!= '$') ||(operands[placeOfReg][0]!= '$') ||(!cheakLongReg(operands,0))||
		(!cheakLongReg(operands,placeOfReg)))
		{
			return printError(line.number,"Unrecognized register");
		}
		if(placeOfIm == 1)
		{
			if(!isInt(operands[placeOfIm]))		
				return printError(line.number,"Operator not an immediate number: %s", operands[placeOfIm]);
		} /* In this situation the operand must be symbol */
		else if(!valid_label(operands[placeOfIm]))
			return printError(line.number,"Operator not a label: %s", operands[placeOfIm]);
		binIOut = MakeIBin(op,fun,operands,*IC,symbolTable);
	}
	else /* J instruction */
	{
		RIJ = 'J';
		if(*op ==STOP_OP)
		{
			if((*operands[0]))/* No operands allowed in stop instruction */
				return printError(line.number,"Incorrect amount of operands.");
		}
		else if(*op <=CALL_OP && *op>=JMP_OP)
		{
			if(!(*operands[0])||!valid_label(operands[0]))
			{
				if((*op!=JMP_OP)||((*op==JMP_OP) &&((operands[0][0]!= '$')||!cheakLongReg(operands,0))))
					return printError(line.number,"Unrecognized register: %s", operands[0]);
			}
		}
		binJOut = MakeJBin(op,fun,operands,symbolTable);
	}
	strtok(line.text + i, " \n\t");
	/* Adds the instruct line to the code image*/
	returnI = addToCodeImg(IC,line.text,codeImg,RIJ);
	/* Adds the binary structure according to the instruction type */
	switch (RIJ)
	{
		case 'R':
			(codeImg[returnI]->word).binR = binROut;
			break;
		case 'I':
			(codeImg[returnI]->word).binI = binIOut;
			break;
		case 'J':
			(codeImg[returnI]->word).binJ = binJOut;
			break;
	}
	return TRUE;
}

bool cheakLongReg(char **operands,int j)
{
	int longReg;
	char stringOp[3];
	if(operands[j][2])
	{
		stringOp[0] = operands[j][1];
		stringOp[1] = operands[j][2];
		if(operands[j][3])
			stringOp[2] = operands[j][3];
		longReg = atoi(stringOp);
		/* Register is bigger than 31 */
		if(longReg>MAX_REG_NUM)
			return FALSE;
	}
	return TRUE;
}
