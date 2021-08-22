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
	signed int immed:16;
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

/* The structure of the code image */
typedef struct codeImage
{
	char type; /* R/I/J */
	int adress;	
	char lineText[MAX_LINE];
	char* attribute;
    union {
		codeBinR *binR;
		codeBinI *binI;
		codeBinJ *binJ;
	} word;
}codeImage;
/* The division of bits for guidance*/
typedef struct dataBin8
{
	signed int number:8;
}dataBin8;
typedef struct dataBin32
{
	signed int number:32;
}dataBin32;
typedef struct dataBin16
{
	signed int number:16;
}dataBin16;
/* The structure of the data image */
typedef struct dataImage
{
	int adress;
	char lineText[MAX_LINE];
	int bytes;
    union {
		dataBin32 *bin32;
		dataBin16 *bin16;
		dataBin8 *bin8;
	}dataBin;
	struct dataImage* next;
}dataImage;

/* The function adds new node for the data image */
void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg,char* temp,int j);
/* The function adds the final IC value to the data image after the first pass */
void updateDataTable(void* dataImg, int DCorIC);
/* The function updates the final values of the address and immediate number to instructions, using symbol table */
void updateBinCodeImg(struct codeImage** codeImg,int ic,int adr,char RIJ,char* text);
/* The function adds new node for the code image */
int addToCodeImg(int *IC, char* line,struct codeImage** codeImg,char RIJ);
/* The function converts registers from string to integer */
int atoiFunc(char* operands);
/* A get function, returns int of address to a specific line of text */
int getAdressCodeImg(struct codeImage** codeImg,char* text,int ic);

#endif

