#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "helpFuncs.h"
#include "opcode.h" 
#include "guidances.h"
#include "globals.h" 
#define ERR_OUTPUT_FILE stderr
bool printError(int lineNum, char* error, ...)
{
	va_list args; 
	fprintf(stdout,"Error in line %d: ",lineNum);
	va_start(args, error);
	vfprintf(stdout, error,args);
	va_end(args);
	fprintf(stdout, "\n");
	return FALSE;


}
void *check_malloc(long size)
{
	void *ptr = malloc(size);
	if (ptr == NULL)
	{
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

char *mallocName(char *s0)
{
	char *str;
	char* exten = strchr(s0,'.');
	if(!exten || strcmp(exten,".as")!=0 ||(strcmp(exten,".as")==0 &&strlen(s0) ==3))   /*File extension/name 				is not ".as"/valid */
		return NULL;
	str = (char *)check_malloc(strlen(s0));
	strcpy(str, s0);
	return str;
}
bool isComment(line line, int i)
{
	if (!line.text[i] || line.text[i] == '\n' || line.text[i] == EOF || line.text[i] == ';')
		return TRUE;
	return FALSE;
}

bool is_label(line line , char* label)
{
	int j=0 , i=0 ;
	SKIP_WHITE_SPACE(line.text , i)
	for (; line.text[i] && line.text[i] != ':' && line.text[i] != EOF && i <= MAX_LINE; i++, j++)
		label[j] = line.text[i];
	label[j] = '\0';
	if (line.text[i] == ':')
	{
		if (!valid_label(label))
		{
			printError(line.number, "Invalid label name - cannot be longer than 32 chars, may only start with letter be alphanumeric.");
			label[0] = '\0';
			return TRUE;
		}
		return FALSE;
	}
	label[0] = '\0';
	return FALSE;
}

bool valid_label(char *a_label)
{
	return(a_label[0] && strlen(a_label) <= 31 && isalpha(a_label[0]) && check_digit_alpha(a_label + 1) &&
	       (!check_reserved_word(a_label)));	
}
	   
bool check_digit_alpha(char *str)
{
	int i;
	for (i=0 ; str[i]; i++) {
		if ((!isalpha(str[i])) && (!isdigit(str[i]))) 
			return FALSE;
	}
	return TRUE;
}

bool check_reserved_word(char *a_label)
{
	int op , fun , gud, reg;
	is_opcode(a_label, &op ,(funct*)&fun);
	is_guidance(a_label, ((guide*)&gud));
	is_register(a_label, &reg);
	if ((op != NONE_OP ) || (gud != NONE_GUIDE) || (reg != NONE_REG))/*is register?*/
		return TRUE;

	return FALSE;
}
void is_register(char *a_label, int *reg)
{
/*get_register_by_name*/
	*reg = NONE_REG;
}

bool isInt(char *str)
{
	int i = 0;
	if (str[0] == '-' || str[0] == '+')
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
		if (*operandCount == 3)
		{
			printError(line.number, "Too many operands for operation (got >%d)", *operandCount);
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


