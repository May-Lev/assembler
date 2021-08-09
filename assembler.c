#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "firstPass.h"
bool cheakLine();
static bool openFile(char *filename);
int main(int argc, char *argv[])
{
	int i;
	bool success = TRUE;
	for (i = 1; i < argc; ++i)
  	{
		if (!success)
			puts(""); 
		success = openFile(argv[i]);
	}
	return 0;
}

static bool openFile(char *filename)
{
	int ic = FIRST_IC, dc=0;
	int code_img=1, dataImg=3;
	struct symbolNode* symbolTable = NULL;
	char *new_filename;
	FILE *pointer_file;
	char temp_line[MAX_LINE + 2];
	bool read_file = TRUE;
	line line;
	new_filename = mallocName(filename);
	pointer_file = fopen(new_filename , "r");
	if(!pointer_file)
	{
		printf("The file \"%s\" is unreadable, we will skip it.\n", filename);
		free(new_filename);
		return FALSE;
	}
	line.text = temp_line;
	for(line.number = 1; fgets(temp_line, MAX_LINE + 2, pointer_file) != NULL; line.number++)
	{
		if(cheakLine(line ,pointer_file))
		{
			if(!readLine(line, &ic, &dc, code_img ,&symbolTable, dataImg))
				read_file = FALSE;
		 } 
		else read_file = FALSE; 
	}
	
	printSymbolTable(symbolTable);
	fclose(pointer_file);
	return read_file;
}

bool cheakLine(line line, FILE * pointer_file)
{ 
	int temp_c;
	if (strchr(line.text, '\n') == NULL && !feof(pointer_file))
	{
		printError(line.number, "Line too long to process. Maximum line length should be %d.",MAX_LINE);
		return FALSE;
			/* skip leftovers */
		do {
			temp_c = fgetc(pointer_file);
		} while (temp_c != '\n' && temp_c != EOF);
	}
	return TRUE;
}
