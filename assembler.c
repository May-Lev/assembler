#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "symbolTable.h"
#include "firstPass.h"
#include "secondPass.h"
#include "outputFiles.h"

/* The function checks the integrity of the opened file */
bool cheakLine();

/* The function goes through every line in the file, in the first and second pass */
static bool openFile(char *filename);

void freeexorEnList(exorEnList* extorEntList);
/*The function iterate over each file*/
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
	int ic = FIRST_IC, dc=0,icf,lineCount,dcf;
	/* Symbol,data and code tables*/
	dataImage* dataImg = NULL;
	codeImage* codeImg[CODE_ARR_IMG_LENGTH];
	struct symbolNode* symbolTable = NULL;
	/* entry/external Lists of labels*/
	exorEnList* externList = NULL;
	exorEnList* enterList = NULL;
	char *newFileName;
	FILE *pointer_file;
	char temp_line[MAX_LINE + 2];
	bool read_file = TRUE;
	line line; 
	/* Open file */
	newFileName = checkFileName(filename);
	pointer_file = fopen(newFileName , "r");
	if(!pointer_file)
	{
		printf("The file %s is unreadable, we will skip it.\n", filename);
		free(newFileName);
		return FALSE;
	}
	line.text = temp_line;
	/* The for prosess each line in the file */
	for(line.number = 1; fgets(temp_line, MAX_LINE + 2, pointer_file) != NULL; line.number++)
	{
		lineCount = line.number;
		if(cheakLine(line ,pointer_file))
		{ /* First pass */
			if(!readLineFirstPass(line, &ic, &dc, codeImg ,&symbolTable, &dataImg))
				read_file = FALSE;
		} 
		else
			read_file = FALSE;
	}

	icf = ic;
	dcf = dc;
	/* If the first pass didn't fail, start the second pass */
	if (read_file)
	{
		ic = FIRST_IC;
		/* Adding the IC value */
		updateSymbolTable(symbolTable, icf, "data");
		updateDataTable(dataImg, icf);
		/* Rewind to the beginning of the file */
		rewind(pointer_file); 
		/* Prosess each line in the file */
		for (line.number = 1;line.number <=lineCount ; line.number++)
		{
			char label[MAX_LINE];
			int i = 0;
			fgets(temp_line, MAX_LINE, pointer_file);
			SKIP_WHITE_SPACE(temp_line , i)
			is_label(line,label);
			if (strlen(label)<1 || temp_line[i] == '.')
			{ /* Second pass */
				read_file &= readLineSecondPass(line, &ic,&icf, codeImg, &symbolTable,&externList,&enterList);
			}
		}
		/* Write files if second pass succeeded */
		if (read_file)
			read_file = writeOutputFiles(codeImg, &dataImg, icf, dcf, newFileName,&externList,&enterList);
		freeexorEnList(externList);
		freeexorEnList(externList);
	}
	fclose(pointer_file);
	return read_file;
}

bool cheakLine(line line, FILE * pointer_file)
{ 
	int temp_c;
	bool result = TRUE;
	if (strchr(line.text, '\n') == NULL && !feof(pointer_file))
	{
		printError(line.number, "Line too long to process. Maximum line length should be %d ",MAX_LINE);
		result =  FALSE;
		do {
			temp_c = fgetc(pointer_file);
		} while (temp_c != '\n' && temp_c != EOF);
	}
	return result;
}
void freeexorEnList(exorEnList* extorEntList)
{
	
}
