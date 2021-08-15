#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "helpFuncs.h"
#include "globals.h" 


void addSymbolTable(struct symbolNode** symbolTable, char* lable, int DCorIC, char* attr)
{
	int i;
	struct symbolNode* new_node = (struct symbolNode*) malloc(sizeof(struct symbolNode));
    	struct symbolNode *last = *symbolTable;
	/*printf("%s",lable);*/
	for(i = 0;i<strlen(lable);i++)
		new_node->symbol[i] = (char)lable[i];
	new_node->value = DCorIC;
	new_node->attribute = attr;
    new_node->next = NULL;
    if (*symbolTable == NULL)
    {
       *symbolTable = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;   
}
void updateSymbolTable(void* symbolTable, int DCorIC, char* attr)
{
	struct symbolNode* currNode;
	for (currNode = symbolTable; currNode != NULL; currNode = currNode->next)
		if(!strcmp(currNode->attribute, attr))
			currNode->value += DCorIC;
}
void printSymbolTable(void* pointer)
{
	int i;
	struct symbolNode *currNode = pointer;
	while (currNode != NULL)
	{
		for(i = 0; i<strlen(currNode->symbol); i++)
			printf("%c", currNode->symbol[i]);
		printf(" %d ", currNode->value);
		printf(" %s\n ", currNode->attribute);
		currNode = currNode->next;
	}
}
int getAdressForLable(char* operand,struct symbolNode** symbolTable)
{
	int adress = -1;
	struct symbolNode *currNode = *symbolTable;
	while (currNode != NULL)
	{
		if(strcmp(currNode->symbol,operand) == 0)
			adress = currNode->value;
		currNode = currNode->next;

	}
	return adress;
}
bool isExternSymbol(char* operand,struct symbolNode** symbolTable)
{
	struct symbolNode *currNode = *symbolTable;
	while (currNode != NULL)
	{
		if(strcmp(currNode->attribute,"external") == 0 && strcmp(currNode->symbol,operand) == 0)
			return TRUE;
		currNode = currNode->next;

	}
	return FALSE;

}
