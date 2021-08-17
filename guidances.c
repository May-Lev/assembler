#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "guidances.h"
#include "globals.h"
#include "symbolTable.h"


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
bool guideHandling(guide guidance,struct symbolNode** symbolTable, char* label, line line,int *DC,int i,dataImage** dataImg,bool SymbolFlag)
{
	bool result = TRUE;
	int bytes;
	if(guidance == DB_GUIDE || guidance == DH_GUIDE ||guidance == DW_GUIDE)
	{
		if(guidance == DB_GUIDE)
			bytes = DB_BYTES;
		if(guidance == DH_GUIDE)
			bytes = DH_BYTES;
		if(guidance == DW_GUIDE)
			bytes = DW_BYTES;
		result = DbDhDwguide(line, i, dataImg, DC/**/,bytes);
	}
	if(guidance == ENTRY_GUIDE)
	{
		if(SymbolFlag)
			return printError(line.number, "Can't define a label to an entry guidance.");
		result = TRUE;
     }
	else if(guidance == EXTERN_GUIDE)
	{	
		int j,k;
		k =i;
		
		for (j = 0; line.text[k] && line.text[k] != '\n' && line.text[k] != '\t'
				 && line.text[k] != ' ' && line.text[k] != EOF; k++, j++)
			{
				label[j] = line.text[k];
			}
			label[j] = '\0';
			if (!valid_label(label))
			{
				printError(line.number, "Invalid external label name: %s", label);
				return TRUE;
			}
			strtok(line.text + i, "\n");
			addSymbolTable(symbolTable, label, 0, "external");
	}
	else if(guidance == ASCIZ_GUIDE)
	{
		result = Ascizguide(line, i, dataImg, DC);
	}
	else if(guidance == ERROR_GUIDE)
		result = FALSE;
	/*if(result)
		addToDataImg(&DC,line.text);*/
	return result;
}
bool Ascizguide(line line, int i,dataImage** dataImg, int *DC)
{
	char temp[80];
	char *last = strrchr(line.text, '"');
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] != '"')
		return printError(line.number, "Missing opening quote of string");
	else if (&line.text[i] == last)
		return printError(line.number, "Missing closing quote of string");
	else
	{
		int j;
		for (j = 0;line.text[i] && line.text[i] != '\n' &&
		       line.text[i] != EOF; i++,j++)
		{
				temp[j] = line.text[i];
		}
		temp[last - line.text] = '\0';
		for(j = 1;temp[j] && temp[j] != '"'; j++)
		{			
			addToDataImg(DC,line.text,0,dataImg,temp,j);
			(*DC)++;
		}
		temp[j] = '\0';
		addToDataImg(DC,line.text,0,dataImg,temp,j);
		(*DC)++;
	}
	return TRUE;
}
bool DbDhDwguide(line line, int i,dataImage** dataImg, int *DC,int bytes)
{

	char temp[80]/*, *temp_ptr*/;
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
			return printError(line.number, "Expected integer for guidance (got '%s')", temp);
		addToDataImg(DC,line.text,bytes,dataImg,temp,-1);
		(*DC)+=bytes;
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == ',')
			i++;
		else if (!line.text[i] || line.text[i] == '\n' || line.text[i] == EOF)
			break;
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == ',')
		{
			return printError(line.number, "Multiple consecutive commas.");
		} 
		else if (line.text[i] == EOF || line.text[i] == '\n' || !line.text[i])
			return printError(line.number, "Missing data after comma");
			
	} while (line.text[i] != '\n' && line.text[i] != EOF);
	return TRUE;
}
