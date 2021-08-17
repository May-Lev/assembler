#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "globals.h"
#define SKIP_WHITE_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;
typedef struct externList
{
	int adress;
	char label[MAX_LINE];
	struct externList* next;
}externList;
void addSymbolTable(struct symbolNode** symbolTable, char* label, int DCorIC, char* attr);
void updateSymbolTable(void* symbolTable, int DCorIC, char* attr);
void printSymbolTable(void* pointer);
int getAdressForLable(char* operand, struct symbolNode** symbolTable);
bool isExternSymbol(char* operand,struct symbolNode** symbolTable);
struct symbolNode* isSymbolInTable(struct symbolNode** symbolTable,char* val);
void addExternLabel(externList** externalList, char* operand, int adress);
void printExternalList(void * pointer);
#endif
