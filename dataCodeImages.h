#ifndef _DATA_CODE_IMAGES_H
#define _DATA_CODE_IMAGES_H
#include "globals.h"


/* The division of bits for R instructions*/
typedef struct codeBinR
{
	unsigned int unuse:6;
	unsigned int funct:5;
	unsigned int rd:5;
	unsigned int rt:5;
	unsigned int rs:5;
	unsigned int opcode:6;
}codeBinR;

/* The division of bits for I instructions*/
typedef struct codeBinI
{
	int immed:16;
	unsigned int rt:5;
	unsigned int rs:5;
	unsigned int opcode:6;
}codeBinI;

/* The division of bits for J instructions*/
typedef struct codeBinJ
{
	unsigned int address:25;
	unsigned int reg:1;
	unsigned int opcode:6;
}codeBinJ;

/* The division of bits for the code image */
typedef struct codeImage
{
	int adress;	
	char lineText[MAX_LINE];
	char* attribute;
    union {
		codeBinR *binR;
		codeBinI *binI;
		codeBinJ *binJ;
		
	} word;
}codeImage;

typedef struct dataBin8
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
}dataBin16;
/* The division of bits for the data image */
typedef struct dataImage
{
	int adress;
	char lineText[MAX_LINE];
	char binChar;
    union {
		dataBin32 *bin32;
		dataBin16 *bin16;
		dataBin8 *bin8;
	}dataBin;
	struct dataImage* next;
}dataImage;

/* The function adds new tab for the data image */
void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg,char* temp,int j);
/* The function adds the final IC value to the data image */
void updateDataTable(void* dataImg, int DCorIC);
/* The function adds new tab for the code image */
int addToCodeImg(int *IC, char* line,struct codeImage** codeImg);
void printDataTable(void* pointer);
void printCodeTable(struct codeImage** codeImg, int IC);
/* The function creates codeBin structure */
codeBinR * MakeRBin(opcode *op, funct *fun, char** operands);
codeBinI * MakeIBin(opcode *op, funct *fun, char** operands, int IC,void* symbolTable);
codeBinJ * MakeJBin(opcode *op, funct *fun, char** operands,void* symbolTable);
/* The function converts numbers from string to integer */
int atoiFunc(char* operands);
int getAdressCodeImg(struct codeImage** codeImg,char* text,int ic);
void updateBinCodeImg(struct codeImage** codeImg,int ic,int adr,char RIJ);
#endif









