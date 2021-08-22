#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "globals.h"
 
/* The structure of a single node of the external or entry list */
typedef struct exorEnList
{
	int adress;
	char label[MAX_LINE];
	struct exorEnList* next;
}exorEnList;

/* The function adds new node for the symbol table */
void addSymbolTable(struct symbolNode** symbolTable, char* label, int DCorIC, char* attr);
/* The function adds the final IC value to the symbol table after the first pass */
void updateSymbolTable(void* symbolTable, int DCorIC, char* attr);
/* A get function, returns int of address to a specific label */
int getAdressForLable(char* operand, struct symbolNode** symbolTable);
/* The function returns wather a label is external or not */
bool isExternSymbol(char* operand,struct symbolNode** symbolTable);
/* The function returns wather a label is in the symbol table or not (returns the symbol node or null) */
struct symbolNode* isSymbolInTable(struct symbolNode** symbolTable,char* val);
/* The function adds new node for the external/entry list */
void addExorEntLabel(exorEnList** extorEntList, char* operand, int adress);

#endif
