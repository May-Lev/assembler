#ifndef _DATA_CODE_IMAGES_H
#define _DATA_CODE_IMAGES_H
#include "globals.h"


/*for R code*/
typedef struct codeBinR
{
	unsigned int unuse:6;
	unsigned int funct:5;
	unsigned int rd:5;
	unsigned int rt:5;
	unsigned int rs:5;
	unsigned int opcode:6;
}codeBinR;

/*for I code*/
typedef struct codeBinI
{
	unsigned int immed:16;
	unsigned int rt:5;
	unsigned int rs:5;
	unsigned int opcode:6;
}codeBinI;

/*for J code*/
typedef struct codeBinJ
{
	unsigned int address:25;
	unsigned int reg:1;
	unsigned int opcode:6;
}codeBinJ;


typedef struct codeImage
{
	int adress;	
	line line;
	char* attribute;
    union {
		codeBinR *binR;
		codeBinI *binI;
		codeBinJ *binJ;
		
	} word;
}codeImage;


/*typedef struct dataBin8
{
	unsigned int number:8;
}dataBin8;
typedef struct dataBin32
{
	unsigned int number:32;
}dataBin32;
typedef struct dataBin16
{
	unsigned int number:16;
}dataBin16;*/
typedef struct dataImage
{
	int adress;
	char lineText[MAX_LINE];
	unsigned int bin8:1;
		/*dataBin32 *bin32;
		dataBin16 *bin16;*/
	struct dataImage* next;
}dataImage;


void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg);
void updateDataTable(void* dataImg, int DCorIC);
void addToCodeImg(int* IC, char* line);
void printDataTable(void* pointer);
#endif







