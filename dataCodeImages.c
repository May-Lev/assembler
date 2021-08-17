#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "dataCodeImages.h"
#include "globals.h"
#include "symbolTable.h"


void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg,char* temp,int j)
{
	int i;
	struct dataImage* new_node = (struct dataImage*) check_malloc(sizeof(struct dataImage));
    struct dataImage *last = *dataImg;
	for(i = 0;i<strlen(line);i++)
		new_node->lineText[i] = (char)line[i];
	/* adding dataBin struct for dh/db/dw guidens, by bytes */
	switch(bytes)
	{
		case 1:
		{
			dataBin8* bin8Out = check_malloc(sizeof(dataBin8));
			bin8Out->number = atoi(temp);
			(new_node->dataBin).bin8 = bin8Out;
			break;
		}
		case 2:
		{
			dataBin16* bin16Out = check_malloc(sizeof(dataBin16));
			bin16Out->number = atoi(temp);
			(new_node->dataBin).bin16 = bin16Out;
			break;
		}
		case 4:
		{
			dataBin32* bin32Out = check_malloc(sizeof(dataBin32));
			bin32Out->number = atoi(temp);
			(new_node->dataBin).bin32 = bin32Out;
			break;
		}
		case 0:
		{
			new_node->binChar = temp[j];
			break;
		}
	}
	new_node->adress = *DC;
    new_node->next = NULL;
    if (*dataImg == NULL)
    {
       *dataImg = new_node;
       return;
    }  
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
    return;   
}
void updateDataTable(void* dataImg, int DCorIC)
{
	struct dataImage* currNode;
	for (currNode = dataImg; currNode != NULL; currNode = currNode->next)
			currNode->adress += DCorIC;
}

int addToCodeImg(int *IC, char* line,struct codeImage** codeImg)
{
	int i,returnI;
	struct codeImage* new_node = (struct codeImage*) check_malloc(sizeof(struct codeImage));	
	for(i = 0;i<strlen(line);i++)
		new_node->lineText[i] = (char)line[i];
	new_node->adress = *IC;
	codeImg[*IC] = new_node;
	returnI = *IC;
	(*IC)+=4;
	return returnI;
}

void printDataTable(void* pointer)
{
	int i;
	struct dataImage *currNode = pointer;
	while (currNode != NULL)
	{
		printf(" %d      ", currNode->adress);
		for(i = 0; i<strlen(currNode->lineText); i++)
			printf("%c", currNode->lineText[i]);
		printf("\n");
		/*if((currNode->dataBin).bin8)
		printf("%d",(currNode->dataBin).bin8->number);
		if((currNode->dataBin).bin16)
		printf("%d",(currNode->dataBin).bin16->number);*/
		currNode = currNode->next;
	}
}
void printCodeTable(struct codeImage** codeImg, int IC)
{
	int i,j;
	for(i = FIRST_IC;i<IC;i+=4)
	{
		printf("%d ",codeImg[i]->adress);
		/*printf("%d ",((codeImg[i]->word).binI->opcode));*/
		for(j = 0; j<strlen(codeImg[i]->lineText); j++)
			printf("%c", codeImg[i]->lineText[j]);
	}
	
}
void updateBinCodeImg(struct codeImage** codeImg,int ic,int adr,char RIJ)
{
	int i;	
	for(i = FIRST_IC;i<ic;i+=4)
	{
		if(RIJ =='J')
			((codeImg[i]->word).binJ)->address = adr;
		if(RIJ =='I')
			((codeImg[i]->word).binI)->immed = adr;
	}

}
codeBinR* MakeRBin(opcode *op, funct *fun,char** operands)
{
	codeBinR* binROut = check_malloc(sizeof(codeBinR));
	binROut->opcode = *op;
	binROut->funct = *fun;
	binROut->unuse = 0;
	if(*op==0)
	{
		binROut->rs = atoiFunc(operands[0]);
		binROut->rt = atoiFunc(operands[1]);
		binROut->rd = atoiFunc(operands[2]);
	}
	else if(*op ==1)
	{
		binROut->rt = 0;
		binROut->rs = atoiFunc(operands[2]);
		binROut->rd = atoiFunc(operands[0]);
	}
	return binROut;
}

codeBinI* MakeIBin(opcode *op, funct *fun,char** operands,int IC,void* symbolTable)
{
	codeBinI* binIOut = check_malloc(sizeof(codeBinI));
	int adressSymbol;
	binIOut->opcode = *op;
	binIOut->rs = atoiFunc(operands[0]);
	if((*op <=NORI_OP && *op>=ADDI_OP) || (*op <=SH_OP && *op>=LB_OP))
	{
		binIOut->immed = atoiFunc(operands[1]);
		binIOut->rt = atoiFunc(operands[2]);
	}
	else
	{
		binIOut->rt = atoiFunc(operands[1]);
		adressSymbol = getAdressForLable(operands[2],symbolTable);
		if(adressSymbol !=-1)
		{
			adressSymbol = adressSymbol-IC;
			binIOut->immed = adressSymbol;
		}
		else
			binIOut->immed = '?';
	}
	return binIOut;
}
codeBinJ* MakeJBin(opcode *op, funct *fun,char** operands,void* symbolTable)
{
	int adressSymbol;
	codeBinJ* binJOut = check_malloc(sizeof(codeBinJ));
	binJOut->opcode = *op;
	binJOut->reg = 0;
	if(*op ==STOP_OP)
		binJOut->address = 0;
	else if((*op==JMP_OP)&&(operands[0][0]=='$'))
	{
		binJOut->reg = 1;
		binJOut->address = atoiFunc(operands[0]);
	}
	else if(*op==LA_OP || *op ==CALL_OP ||*op==JMP_OP)
	{
		adressSymbol = getAdressForLable(operands[0],symbolTable);
		if(adressSymbol !=-1)
		{
			if(!isExternSymbol(operands[0],symbolTable))
				binJOut->address = adressSymbol;
			else
				binJOut->address = 0;
		}
		else
		{
			/*printf("%s\n",*operands);*/
			binJOut->address = '?';
		}
	}
	return binJOut;
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
	int adress = -1,i;
	for(i = FIRST_IC;i<ic;i+=4)
	{
		/*printf("%s %s", text,codeImg[i]->lineText);*/
		if(strcmp(text,codeImg[i]->lineText)==0)
		{
		adress = codeImg[i]->adress;
		return adress;	
		}
	}
	
	return adress;

}
