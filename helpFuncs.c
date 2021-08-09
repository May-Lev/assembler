#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "helpFuncs.h"
#include "opcode.h" 
#include "instructions.h"
#include "globals.h" 
#define ERR_OUTPUT_FILE stderr
extern bool SymbolFlag;
int printError(int lineNum, char* error, ...)
{
	int out;
	va_list args; 
	fprintf(stdout,"Error in line %d: ",lineNum);
	va_start(args, error);
	out = vfprintf(stdout, error,args);
	va_end(args);
	fprintf(stdout, "\n");
	return out;


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
	int op , fun , ins, reg;
	is_opcode(a_label, &op ,(funct*)&fun);
	is_instruction(a_label, ((instruct*)&ins));
	is_register(a_label, &reg);
	if ((op != NONE_OP ) || (ins != NONE_INS) || (reg != NONE_REG))/*is register?*/
		return TRUE;

	return FALSE;
}
void is_register(char *a_label, int *reg)
{
/*get_register_by_name*/
	*reg = NONE_REG;
}


void addSymbolToDataImg(struct symbolNode** pointerList, char* lable, int DCorIC, char* attr)
{
	int i;
	struct symbolNode* new_node = (struct symbolNode*) malloc(sizeof(struct symbolNode));
    struct symbolNode *last = *pointerList;
	for(i = 0;i<strlen(lable);i++)
		new_node->symbol[i] = (char)lable[i];
	new_node->value = DCorIC;
	new_node->attribute = attr;
    new_node->next = NULL;
    if (*pointerList == NULL)
    {
       *pointerList = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;   
}

void printDataImg(void* pointer)
{
	int i;
	struct symbolNode *last = pointer;
	while (last != NULL)
	{
		for(i = 0;i<strlen(last->symbol);i++)
			printf("%c", last->symbol[i]);
		printf(" %d ", last->value);
		printf(" %s\n ", last->attribute);
		last = last->next;
	}
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





