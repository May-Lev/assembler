#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "instructions.h"
#include "globals.h"

bool instructHandling(char* instructName, line line,int *IC,int i,codeImage** codeImg,opcode *op, funct *fun, char ** operands)
{
	int j, numReg = 2,placeOfIm = 2,placeOfReg = 1;
	if(*op == NONE_OP)
	{
		return printError(line.number,"Unrecognized instruction: %s.", instructName);
	}
	if(*op ==1|| *op==0)
	{
		if(*op==0)
			numReg =3;
		for(j = 0; j<numReg; j++)
		{	
			if(!(*operands[j]))
			{
				return printError(line.number,"Incorrect amount of operands.");
			}
			if((operands[j][0]!= '$')||(operands[j][3])||!cheakLongReg(operands,j))
			{
				return(printError(line.number,"Unrecognized register: %s", operands[j]));
			}
		}
	addToCodeImg(IC,line.text,codeImg);
	}
	else if(*op <=24 && *op>=10)
	{
		if((*op <=14 && *op>=10) || (*op <=24 && *op>=19))
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
		}
		else if(!valid_label(operands[placeOfIm]))
		{
			return printError(line.number,"Operator not a label: %s", operands[placeOfIm]);
		}
	addToCodeImg(IC,line.text,codeImg);
	}
	else/* if((*op <=32 && *op>=30)|| *op ==63)*/
	{
		if(*op ==63)
		{
			if((*operands[0]))
			{
				return printError(line.number,"Incorrect amount of operands.");
			}
		}
		else if(*op <=32 && *op>=30)
		{
			if(!(*operands[0])||!valid_label(operands[0]))
			{
				if((*op!=30)||((*op==30) &&((operands[0][0]!= '$')||!cheakLongReg(operands,0))))
				{
					return printError(line.number,"Unrecognized register: %s", operands[0]);
				}
			}
		}
		addToCodeImg(IC,line.text,codeImg);
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
		if(longReg>MAX_REG_NUM)
			return FALSE;
	}
	return TRUE;
}



