#include <string.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "dataCodeImages.h"
#include "globals.h"
#include "symbolTable.h"

codeBinR* MakeRBin(opcode *op, funct *fun,char** operands)
{
	codeBinR* binROut = check_malloc(sizeof(codeBinR));
	binROut->opcode = *op;
	binROut->funct = *fun;
	binROut->unuse = 0;
	if(*op==0)/* Arithmetic and logical instructions (R) */
	{
		binROut->rs = atoiFunc(operands[0]);
		binROut->rt = atoiFunc(operands[1]);
		binROut->rd = atoiFunc(operands[2]);
	}
	else if(*op ==1) /* Copy instructions (R) */
	{
		binROut->rt = 0;
		binROut->rs = atoiFunc(operands[0]);
		binROut->rd = atoiFunc(operands[1]);
	}
	return binROut;
}

codeBinI* MakeIBin(opcode *op, funct *fun,char** operands,int IC,void* symbolTable)
{
	codeBinI* binIOut = check_malloc(sizeof(codeBinI));
	int adressSymbol;
	binIOut->opcode = *op;
	binIOut->rs = atoiFunc(operands[0]);
	if((*op <=NORI_OP && *op>=ADDI_OP) || (*op <=SH_OP && *op>=LB_OP))
	{
		binIOut->immed = atoi(operands[1]);
		binIOut->rt = atoiFunc(operands[2]);
	}
	else /* Conditional branching instructions (I) */
	{	
		binIOut->rt = atoiFunc(operands[1]);
		adressSymbol = getAdressForLable(operands[2],symbolTable);
		if(adressSymbol != NO_ADDRESS)
		{
			adressSymbol = adressSymbol-IC;
			binIOut->immed = adressSymbol;
		}
		else /* unknown label address, will be updated in the second pass */
			binIOut->immed = '?';
	}
	return binIOut;
}
codeBinJ* MakeJBin(opcode *op, funct *fun,char** operands,void* symbolTable)
{
	int adressSymbol;
	codeBinJ* binJOut = check_malloc(sizeof(codeBinJ));
	binJOut->opcode = *op;
	binJOut->reg = 0;
	if(*op ==STOP_OP)
		binJOut->address = 0;
	else if((*op==JMP_OP)&&(operands[0][0]=='$')) /* Jmp instruction using register */
	{
		binJOut->reg = 1;
		binJOut->address = atoiFunc(operands[0]);
	}
	else if(*op==LA_OP || *op ==CALL_OP ||*op==JMP_OP)
	{
		adressSymbol = getAdressForLable(operands[0],symbolTable);
		if(adressSymbol != NO_ADDRESS)
		{
			if(!isExternSymbol(operands[0],symbolTable))
				binJOut->address = adressSymbol;
			else
				binJOut->address = 0;
		}
		else /* unknown label address, will be updated in the second pass */
			binJOut->address = '?';
	}
	return binJOut;
}
