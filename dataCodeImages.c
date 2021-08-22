#include <string.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "dataCodeImages.h"
#include "globals.h"
#include "symbolTable.h"
#include "bitBuild.h"

void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg,char* temp,int j)
{
	int i;
	struct dataImage* new_node = (struct dataImage*) check_malloc(sizeof(struct dataImage));
    struct dataImage *last = *dataImg;
	for(i = 0;i<strlen(line);i++)
		new_node->lineText[i] = (char)line[i];
	new_node->bytes = bytes;
	/* adding dataBin struct for dh/db/dw guidens, by bytes */
	switch(bytes)
	{
		case DB_BYTES:
		{
			dataBin8* bin8Out = check_malloc(sizeof(dataBin8));
			bin8Out->number = atoi(temp);
			(new_node->dataBin).bin8 = bin8Out;
			break;
		}
		case DH_BYTES:
		{
			dataBin16* bin16Out = check_malloc(sizeof(dataBin16));
			bin16Out->number = atoi(temp); /* String to int */
			(new_node->dataBin).bin16 = bin16Out;
			break;
		}
		case DW_BYTES:
		{
			dataBin32* bin32Out = check_malloc(sizeof(dataBin32));
			bin32Out->number = atoi(temp); /* String to int */
			(new_node->dataBin).bin32 = bin32Out;
			break;
		}
		case 0:/*asciz guide*/
		{
			dataBin8* bin8Out = check_malloc(sizeof(dataBin8));
			bin8Out->number = (int)(temp[j]); /* Char to int */
			(new_node->dataBin).bin8 = bin8Out;
			break;
		}
	}
	new_node->adress = *DC;
    new_node->next = NULL;
    if (*dataImg == NULL) /* First node */
    {
       *dataImg = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}
void updateDataTable(void* dataImg, int DCorIC)
{
	struct dataImage* currNode;
	for (currNode = dataImg; currNode != NULL; currNode = currNode->next)
		currNode->adress += DCorIC;
}

int addToCodeImg(int *IC, char* line,struct codeImage** codeImg,char RIJ)
{
	int i,returnI;
	struct codeImage* new_node = (struct codeImage*)check_malloc(sizeof(struct codeImage));	
	for(i = 0;i<strlen(line);i++)
		new_node->lineText[i] = (char)line[i];
	new_node->type = RIJ;
	new_node->adress = *IC;
	/* Adds the new node to the array of code image */
	codeImg[*IC] = new_node;
	returnI = *IC;
	/* Update the ic counter */
	(*IC)+=IC_JUMP;
	/* Return the ic counter of this node (before update) */
	return returnI;
}

void updateBinCodeImg(struct codeImage** codeImg,int ic,int adr,char RIJ,char* text)
{
	int i;
	for(i = FIRST_IC;i<ic;i+=IC_JUMP)
	{
		if(strcmp(text,codeImg[i]->lineText)==0)
		{
			if(RIJ =='J')
				((codeImg[i]->word).binJ)->address = adr;
			if(RIJ =='I')
				((codeImg[i]->word).binI)->immed = adr;
			return;
		}
	}
}

int atoiFunc(char* operands)
{
	char str[2];
	str[0] = operands[1];
	str[1] = operands[2];
	return atoi(str);
}
int getAdressCodeImg(struct codeImage** codeImg,char* text,int ic)
{
	int adress = NO_ADDRESS,i;
	for(i = FIRST_IC;i<ic;i+=IC_JUMP)
	{
		if(strcmp(text,codeImg[i]->lineText)==0)
		{
		adress = codeImg[i]->adress;
		return adress;	
		}
	}
	return adress;
}
