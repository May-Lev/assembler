#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "helpFuncs.h"
#include "opcode.h" 
#include "guidances.h"
#include "globals.h" 

bool printError(int lineNum, char* error, ...)
{
	va_list args; 
	fprintf(stdout,"Error in line %d: ",lineNum);
	va_start(args, error);
	vfprintf(stdout, error,args);
	va_end(args);
	fprintf(stdout, "\n"); /* Prints to the stdout */
	return FALSE;
}
void *check_malloc(long size)
{
	void *ptr = malloc(size+1);
	if (ptr == NULL) /* malloc failed */
	{
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

char *checkFileName(char *str)
{
	char *stringName;
	char* exten = strchr(str,'.');
	if(!exten || strcmp(exten,".as")!=0 ||(strcmp(exten,".as")==0 &&strlen(str) ==3)) /*File extension/name 				is not ".as"/valid */
		return NULL;
	stringName = (char *)check_malloc(strlen(str)); /* Allocate memory for the file name */
	strcpy(stringName, str);
	return stringName;
}
char *addExtensionToFile( char *filename, char *fileExtension)
{
	char *retStr;
    char *lastExt;
	char *newFileName;
    if ((retStr = malloc(strlen (filename) + 1)) == NULL) return FALSE;
   	strcpy (retStr, filename);
    lastExt = strrchr(retStr, '.');
   	if (lastExt != NULL)
       	*lastExt = '\0';
	/* Allocate memory for the file name and extension */
	newFileName  = (char*)check_malloc(strlen(retStr) + strlen(fileExtension) + 1);
	strcpy(newFileName, retStr);
	strcat(newFileName, fileExtension);
	return newFileName;
}

bool isComment(line line, int i)
{
	if (!line.text[i] || line.text[i] == '\n' || line.text[i] == EOF || line.text[i] == ';')
		return TRUE;
	return FALSE;
}

bool is_label(line line , char* label)
{
	int j=0, i=0;
	SKIP_WHITE_SPACE(line.text , i)
	for (; line.text[i] && line.text[i] != ':' && line.text[i] != EOF && i <= MAX_LINE; i++, j++)
		label[j] = line.text[i];
	label[j] = '\0';
	if (line.text[i] == ':')/* Label mast end with colon */
	{
		if (!valid_label(label)) /* Is label valid */
		{
			printError(line.number, "Invalid label name - cannot be longer than 32 chars, may only start with letter be alphanumeric, and cannot be a saved word.");
			label[0] = '\0';
			return TRUE;
		}
		return FALSE;
	}
	label[0] = '\0';
	return FALSE;
}

bool valid_label(char *label)
{
	if(label[0] && label[0]=='$')
		label = label+1;
	return(label[0] && strlen(label) <= 31 && isalpha(label[0]) && checkDigitAlpha(label) &&
	       (!savedWord(label)));	
}
	
bool checkDigitAlpha(char *str)
{
	int i;
	for (i=0 ; str[i]; i++)
	{
		if ((!isalpha(str[i])) && (!isdigit(str[i]))) /* Not an alphanumeric char */
			return FALSE;
	}
	return TRUE;
}

bool savedWord(char *label)
{
	int op , fun , gud;
	is_opcode(label, &op ,(funct*)&fun);
	is_guidance(label, ((guide*)&gud));
	if ((op != NONE_OP ) || (gud != NONE_GUIDE)) /* The word is already used, an instruction/guidance name */
		return TRUE;
	return FALSE;
}
bool isInt(char *str)
{
	int i = 0;
	if (str[0] == '-' || str[0] == '+') /* A signed number */
		str++;
	for (; str[i]; i++)
	{
		if (!isdigit(str[i]))
			return FALSE;
	}
	return i > 0;
}
bool operandsCheck(line line, int i, char **operands, int *operandCount, char *instructName)
{
	int j;
	int k;
	*operandCount = 0;
	operands[0] = operands[1] = operands[2] = "";
	SKIP_WHITE_SPACE(line.text, i)
	if (line.text[i] == ',')
	{
		printError(line.number, "Unexpected comma after command.");
		return FALSE;
	}
	for (*operandCount = 0; line.text[i] != EOF && line.text[i] != '\n' && line.text[i];)
	{
		if (*operandCount == MAX_OPERANDS_INST)/* Too many operands */
		{
			printError(line.number, "Too many operands for operation (more than >%d)", *operandCount);
			free(operands[0]);
			free(operands[1]);
			free(operands[2]);
			return FALSE;
		}
		operands[*operandCount] = check_malloc(MAX_LINE);
		for (j = 0; line.text[i] && line.text[i] != '\t' && line.text[i] != ' ' && line.text[i] != '\n' && line.text[i] != EOF &&  line.text[i] != ','; i++, j++)
		{
			operands[*operandCount][j] = line.text[i];
		}
		operands[*operandCount][j] = '\0';
		(*operandCount)++;
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == '\n' || line.text[i] == EOF || !line.text[i])
			break;
		else if (line.text[i] != ',')
		{
			printError(line.number, "Expecting ',' between operands");
			for(k = 0; k < *operandCount ; k++)
				free(operands[k]);
			return FALSE;
		}
		i++;
		SKIP_WHITE_SPACE(line.text, i)
		if (line.text[i] == '\n' || line.text[i] == EOF || !line.text[i])
			printError(line.number, "Missing operand after comma.");
		else if (line.text[i] == ',')
			printError(line.number, "Multiple consecutive commas.");
		else continue; 
		{ 
			for(k = 0; k < *operandCount ; k++)
			{
				free(operands[k]);
			}
			return FALSE;
		}
	}
	return TRUE;
}

