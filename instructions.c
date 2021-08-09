#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "instructions.h"
#include "globals.h"


/*struct divi_instruct {
	char *name_ins;
	instruct ins;
};

static struct divi_instruct instruct_tabel[] = {
	
		{"db", 	 DB_INS},
		{"dh",   DH_INS},
		{"dw",   DW_INS},
		{"asciz",  ASCIZ_INS},
		{"entry",  ENTRY_INS},
		{"extern", EXTERN_INS},
		{NULL, NONE_INS}
};*/

struct instructFindTable
{
	char *name;
	instruct value;
};
static struct instructFindTable
		instructTabel[] = {
		{"db", 	 DB_INS},
		{"dh",   DH_INS},
		{"dw",   DW_INS},
		{"asciz",  ASCIZ_INS},
		{"entry",  ENTRY_INS},
		{"extern", EXTERN_INS},
		{NULL, NONE_INS}
};






instruct instructionByName(char *name)
{
	struct instructFindTable *curr_item;
	for (curr_item = instructTabel; curr_item->name != NULL; curr_item++)
	{
		if (strcmp(curr_item->name, name) == 0)
			return curr_item->value;
	}
	return NONE_INS;
}



/*check if the word is instruction*/
void is_instruction(char *name, instruct *get_ins)
{
	struct instructFindTable *b;
	*get_ins = NONE_INS;
	for (b = instructTabel ; b->name != NULL; b++)
	{
		if ( strcmp( b->name , name) == 0)
		{
			*get_ins = b->value;
			return;
		}
	}
	
}

instruct instructionByIndex(line line, int *i)
{
	char temp[MAX_LINE];
	int j;
	instruct ints;
	SKIP_WHITE_SPACE(line.text, *i)
	if(line.text[*i] != '.')
		return NONE_INS;
	for (j = 0; line.text[*i] && line.text[*i] != '\t' && line.text[*i] != ' '; (*i)++, j++)
		temp[j] = line.text[*i];
	temp[j] = '\0';
	ints = (instruct)(temp+1);
	if ((ints = instructionByName(temp+1)) != NONE_INS)
		return ints;
	printError(line.number,"Invalid instruction name: %s", temp);
	return ERROR_INS;
}
bool instructHandling(instruct instruction,struct symbolNode** symbolTable, char* label, line line,int DC,int i,int dataImg)
{
	bool result = TRUE;
	int bytes;
	/*NONE_INS = 0*/	
	if(instruction == DB_INS || instruction == DH_INS ||instruction == DW_INS)
	{
		if(label[0] != '\0')
				addSymbolTable(symbolTable, label, DC, "data");
		if(instruction == DB_INS)
			bytes = 1;
		if(instruction == DH_INS)
			bytes = 2;
		if(instruction == DW_INS)
			bytes = 4;
		DbDhDwinstruct(line, i, dataImg, DC/**/,bytes);
	}
	
	switch(instruction)
	{
    	case DB_INS:
		{
      		break;
		}
		case DH_INS:
		{
      		break;
		}
		case DW_INS:
		{
      		break;
		}
		case ASCIZ_INS:
		{
      		break;
		}
		case ENTRY_INS:
		{
			if(label[0] != '\0')
			printError(line.number, "Can't define a label to an entry instruction.");
			result = FALSE;
      		break;
		}
		case EXTERN_INS:
		{
		/**/
			addSymbolTable(symbolTable, label, 0, "extern");
      		break;
		}
		case NONE_INS:
		case ERROR_INS:
		{
			result = FALSE;
      		break;
		}


	}
	return result;
}
bool DbDhDwinstruct(line line, int i,/*struct symbolNode***/int dataImg, int DC,int bytes)
{

	char temp[80], *temp_ptr;
	long value;
	int j;
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == ',')
	{
		printError(line.number, "Unexpected comma after .data instruction");
		/*return FALSE;*/ /*למה אין*/
		}
	do {
		for (j = 0;
		     line.text[i] && line.text[i] != EOF && line.text[i] != '\t' &&
		     line.text[i] != ' ' && line.text[i] != ',' &&
		     line.text[i] != '\n'; i++, j++)
		{
			 temp[j] = line.text[i];
		}
		temp[j] = '\0';
		if (!isInt(temp))
		{
			printError(line.number, "Expected integer for .data instruction (got '%s')", temp);
			return FALSE;
		}
		value = strtol(temp, &temp_ptr, 10);
		printf("%ld\n",value);
		/*dataImg[*DC] = value;
		(DC)++;*/
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == ',')
			i++;
		else if (!line.text[i] || line.text[i] == '\n' || line.text[i] == EOF)
			break;
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == ',')
		{
			printError(line.number, "Multiple consecutive commas.");
			return FALSE;
		} else if (line.text[i] == EOF || line.text[i] == '\n' || !line.text[i]) {
			printError(line.number, "Missing data after comma");
			return FALSE;
		}
	} while (line.text[i] != '\n' && line.text[i] != EOF);
	return TRUE;
}
