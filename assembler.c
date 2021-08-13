#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "symbolTable.h"
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
	int ic = FIRST_IC, dc=0,icf,dcf;
	dataImage* dataImg = NULL;
	codeImage* codeImg[CODE_ARR_IMG_LENGTH];
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
			if(!readLine(line, &ic, &dc, codeImg ,&symbolTable, &dataImg))
			{
				read_file = FALSE;
				/*icf = -1;*/
			}
		 } 
		else
			read_file = FALSE;
	}

	icf = ic;
	dcf = dc;
	/* if first pass didn't fail, start the second pass */
	if (read_file)
	{
		ic = FIRST_IC;
		/* Now let's add IC to each DC for each of the data symbols in table (step 1.19) */
		updateSymbolTable(symbolTable, icf, "data");
		updateDataTable(dataImg, icf);
		/*printSymbolTable(symbolTable);
		printDataTable(dataImg);*/
		/* start second pass: */
		rewind(pointer_file); /* Start from beginning of file again */

	}
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
