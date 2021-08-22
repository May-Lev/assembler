#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "helpFuncs.h"
#include "globals.h" 
#include "symbolTable.h"

void addSymbolTable(struct symbolNode** symbolTable, char* lable, int DCorIC, char* attr)
{
	int i;
	struct symbolNode* new_node = (struct symbolNode*)check_malloc(sizeof(struct symbolNode));
    	struct symbolNode *last = *symbolTable;
	for(i = 0;i<strlen(lable);i++)
		new_node->symbol[i] = (char)lable[i];
	new_node->value = DCorIC;
	new_node->attribute = attr;
    new_node->next = NULL;
    if (*symbolTable == NULL)/* First node */
    {
       *symbolTable = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}
void updateSymbolTable(void* symbolTable, int DCorIC, char* attr)
{
	struct symbolNode* currNode;
	for (currNode = symbolTable; currNode != NULL; currNode = currNode->next)
		if(!strcmp(currNode->attribute, attr)) /* Adds the value only for a specific attribute */
			currNode->value += DCorIC;
}

int getAdressForLable(char* operand,struct symbolNode** symbolTable)
{
	int adress = NO_ADDRESS;
	struct symbolNode *currNode = *symbolTable;
	while (currNode != NULL)
	{
		if(strcmp(currNode->symbol,operand) == 0) /* Adress was found */
		{
			adress = currNode->value;
			return adress;
		}
		currNode = currNode->next;
	}
	return adress; /* Adress was not found */
}
bool isExternSymbol(char* operand,struct symbolNode** symbolTable)
{
	struct symbolNode *currNode = *symbolTable;
	while (currNode != NULL)
	{
		if(strcmp(currNode->attribute,"external") == 0 && strcmp(currNode->symbol,operand) == 0)
			return TRUE; /* Symbol was found and it is external */
		currNode = currNode->next;
	}
	return FALSE;
}
struct symbolNode* isSymbolInTable(struct symbolNode** symbolTable,char* labelEntr)
{
	struct symbolNode *currNode = *symbolTable;
	while (currNode != NULL)
	{
		if(strcmp(currNode->symbol,labelEntr) == 0) /* Symbol was found */
			return currNode;
		currNode = currNode->next;
	}
	return NULL;
}
void addExorEntLabel(exorEnList** extorEntList, char* operand, int adress)
{
	int i;
	struct exorEnList* new_node = (struct exorEnList*)check_malloc(sizeof(struct exorEnList));
	struct exorEnList *last = *extorEntList;
	for(i = 0;i<strlen(operand);i++)
		new_node->label[i] = (char)operand[i];
	new_node->adress = adress;
    new_node->next = NULL;
    if (*extorEntList == NULL) /* First node */
    {
       *extorEntList = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}
