#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "instructions.h"
#include "globals.h"

bool instructHandling(char* instructName, line line,int IC,int i,codeImage** codeImg,opcode *op, funct *fun)
{
	/*if(*op ==1|| *op==0)
		printf("R fun");
	else if(*op <=24 && *op>=10)
		printf("I");
	else if((*op <=32 && *op>=30)|| *op ==63)
		printf("J");*/
	if(*op == NONE_OP)
	{
		printError(line.number,"Unrecognized instruction: %s.", instructName);
		return FALSE;
	}
	return TRUE;
}
