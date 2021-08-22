#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include "opcode.h"
#include "helpFuncs.h"

/* A struct of a opcode tabel */
struct divi_opcode
{
	char *name_op;
	opcode op;
	funct fun;
};

/* A tabel for opcode string */
static struct divi_opcode opcode_tabel[] =
{
	{"add",ADD_OP, ADD_FUNCT},
	{"sub",SUB_OP, SUB_FUNCT},
	{"and",AND_OP, AND_FUNCT},
	{"or",OR_OP, OR_FUNCT},
	{"nor",NOR_OP, NOR_FUNCT},
	{"move",MOVE_OP, MOVE_FUNCT},
	{"mvhi",MVHI_OP, MVHI_FUNCT},
	{"mvlo",MVLO_OP, MVLO_FUNCT},
	{"addi",ADDI_OP, NONE_FUNCT},
	{"subi",SUBI_OP, NONE_FUNCT},
	{"andi",ANDI_OP, NONE_FUNCT},
	{"ori",ORI_OP, NONE_FUNCT},
	{"nori",NORI_OP, NONE_FUNCT},
	{"bne",BNE_OP, NONE_FUNCT},
	{"beq",BEQ_OP, NONE_FUNCT},
	{"blt",BLT_OP, NONE_FUNCT},
	{"bgt",BGT_OP, NONE_FUNCT},
	{"lb",LB_OP, NONE_FUNCT},
	{"sb",SB_OP, NONE_FUNCT},
	{"lw",LW_OP, NONE_FUNCT},
	{"sw",SW_OP, NONE_FUNCT},
	{"lh",LH_OP, NONE_FUNCT},
	{"sh",SH_OP, NONE_FUNCT},
	{"jmp",JMP_OP, NONE_FUNCT},
	{"la",LA_OP, NONE_FUNCT},
	{"call",CALL_OP, NONE_FUNCT},
	{"stop",STOP_OP, NONE_FUNCT},
	{NULL, NONE_OP, NONE_FUNCT}
};

void is_opcode(char *name_op, opcode *get_op, funct *get_fun)
{
	struct divi_opcode *a;
	/* If name wasnt found in divi_opcode */
	*get_op = NONE_OP;
	*get_fun = NONE_FUNCT;
	for (a = opcode_tabel; a->name_op != NULL; a++)
	{
		if (strcmp(a->name_op,name_op) == 0)
		{
			*get_op = a->op;
			*get_fun = a->fun;
			return;
		}
	}
}
