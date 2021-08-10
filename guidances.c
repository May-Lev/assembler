#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "guidances.h"
#include "globals.h"




struct guideFindTable
{
	char *name;
	guide value;
};
static struct guideFindTable
		guideTabel[] = {
		{"db", 	 DB_GUIDE},
		{"dh",   DH_GUIDE},
		{"dw",   DW_GUIDE},
		{"asciz",  ASCIZ_GUIDE},
		{"entry",  ENTRY_GUIDE},
		{"extern", EXTERN_GUIDE},
		{NULL, NONE_GUIDE}
};






guide guidanceByName(char *name)
{
	struct guideFindTable *curr_item;
	for (curr_item = guideTabel; curr_item->name != NULL; curr_item++)
	{
		if (strcmp(curr_item->name, name) == 0)
			return curr_item->value;
	}
	return NONE_GUIDE;
}



/*check if the word is guidance*/
void is_guidance(char *name, guide *get_guide)
{
	struct guideFindTable *b;
	*get_guide = NONE_GUIDE;
	for (b = guideTabel ; b->name != NULL; b++)
	{
		if ( strcmp( b->name , name) == 0)
		{
			*get_guide = b->value;
			return;
		}
	}
	
}

guide guidanceByIndex(line line, int *i)
{
	char temp[MAX_LINE];
	int j;
	guide guidance;
	SKIP_WHITE_SPACE(line.text, *i)
	if(line.text[*i] != '.')
		return NONE_GUIDE;
	for (j = 0; line.text[*i] && line.text[*i] != '\t' && line.text[*i] != ' '; (*i)++, j++)
		temp[j] = line.text[*i];
	temp[j] = '\0';
	guidance = (guide)(temp+1);
	if ((guidance = guidanceByName(temp+1)) != NONE_GUIDE)
		return guidance;
	printError(line.number,"Invalid guidance name: %s", temp);
	return ERROR_GUIDE;
}
bool guideHandling(guide guidance,struct symbolNode** symbolTable, char* label, line line,int DC,int i,dataImage** dataImg)
{
	bool result = TRUE;
	int bytes;
	if(label[0] != '\0')
			addSymbolTable(symbolTable, label, DC, "data");
	/*NONE_GUIDE = 0*/	
	if(guidance == DB_GUIDE || guidance == DH_GUIDE ||guidance == DW_GUIDE)
	{
		if(guidance == DB_GUIDE)
			bytes = DB_BYTES;
		if(guidance == DH_GUIDE)
			bytes = DH_BYTES;
		if(guidance == DW_GUIDE)
			bytes = DW_BYTES;
		DbDhDwguide(line, i, dataImg, DC/**/,bytes);
	}
	
	switch(guidance)
	{
    	case DB_GUIDE:
		{
      		break;
		}
		case DH_GUIDE:
		{
      		break;
		}
		case DW_GUIDE:
		{
      		break;
		}
		case ASCIZ_GUIDE:
		{
      		break;
		}
		case ENTRY_GUIDE:
		{
			if(label[0] != '\0')
			printError(line.number, "Can't define a label to an entry guidance.");
			result = FALSE;
      		break;
		}
		case EXTERN_GUIDE:
		{
		/**/
			addSymbolTable(symbolTable, &line.text[i], 0, "external");
      		break;
		}
		case NONE_GUIDE:
		case ERROR_GUIDE:
		{
			result = FALSE;
      		break;
		}


	}
	return result;
}
bool DbDhDwguide(line line, int i,dataImage** dataImg, int DC,int bytes)
{

	char temp[80], *temp_ptr;
	/*long value;*/
	int j;
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == ',')
	{
		printError(line.number, "Unexpected comma after guidance");
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
			printError(line.number, "Expected integer for guidance (got '%s')", temp);
			return FALSE;
		}
		/*value = strtol(temp, &temp_ptr, 10);
		dataImg[DC] = value;*/
		(DC)++;
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
