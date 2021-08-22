#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "outputFiles.h"
#include "helpFuncs.h"
#include "symbolTable.h"

bool writeOutputFiles(codeImage** codeImg,dataImage** dataImg, int icf, int dcf, char *filename,exorEnList** externList,exorEnList** enterList)
{
	bool isSuccess;
	/* Making the files */
	isSuccess = (writeExtEntFile(externList, filename, ".ext") && 
			writeExtEntFile(enterList, filename, ".ent")&&
			writeObjFile(codeImg, dataImg, icf, dcf, filename,".ob"));
	return isSuccess; /* No errors occurred in the making of the files (not necessarily that all files were made) */
}

bool writeExtEntFile(exorEnList** exttorEnrList, char *filename, char *fileExtension)
{
	/* concatenate filename and extension, and open the file for writing: */
	FILE *filePointer;
	int i;
	char *newFileName;
	struct exorEnList *currNode = *exttorEnrList;
	/* if list is null, no need to make a file */
	if (!(*exttorEnrList))
		return TRUE;
	newFileName = addExtensionToFile(filename,fileExtension);
	/* Try to open the file for writing */
	filePointer = fopen(newFileName, "w");
	free(newFileName);
	/* if failed, print error and exit */
	if (filePointer == NULL) {
		printf("Can't create or rewrite to file %s", filename);
		return FALSE;
	}
	while (currNode != NULL) /* Write into the file */
	{
		for(i = 0; i<strlen(currNode->label); i++)
			fprintf(filePointer,"%c", currNode->label[i]);
		fprintf(filePointer," %04d\n", currNode->adress);
		currNode = currNode->next;
	}
	fclose(filePointer);
	return TRUE;
}

bool writeObjFile(codeImage** codeImg,dataImage** dataImg, int icf, int dcf,char *filename,char* fileExtension)
{
	int i,j;	
    unsigned int tempbit = 0, valCodeImg, valDataImg;
	FILE *file_desc;
	struct dataImage *dataImgNode = *dataImg;
	/* add extension of file to open */
	char *output_filename = addExtensionToFile(filename,fileExtension);
	/* Try to open the file for writing */
	file_desc = fopen(output_filename, "w");
	free(output_filename);
	if (file_desc == NULL) {
		printf("Can't create or rewrite to file %s", output_filename);
		return FALSE;
	}
	/* Write the first line of the file */
	fprintf(file_desc, "   %d %d\n", icf - FIRST_IC, dcf);

	/* Write code image */
	for (i = FIRST_IC;i<icf;i+=IC_JUMP)
	{
		if (codeImg[i]->type == 'R')
		{
		 	valCodeImg = codeImg[i]->word.binR->opcode; 
			tempbit |= (valCodeImg << 26); 
			valCodeImg = codeImg[i]->word.binR->rs;
			tempbit |= (valCodeImg << 21);
			valCodeImg = codeImg[i]->word.binR->rt;
			tempbit |= (valCodeImg << 16); 
			valCodeImg = codeImg[i]->word.binR->rd;
			tempbit |= (valCodeImg << 11); 
			valCodeImg = codeImg[i]->word.binR->funct;        
			tempbit |= (valCodeImg << 6); 
			valCodeImg = codeImg[i]->word.binR->unuse;
			tempbit |= (valCodeImg); 
		}
		else if (codeImg[i]->type == 'I')
		{
			valCodeImg = codeImg[i]->word.binI->opcode; 
			tempbit |= (valCodeImg << 26); 
			valCodeImg = codeImg[i]->word.binI->rs;
			tempbit |= (valCodeImg << 21);
			valCodeImg = codeImg[i]->word.binI->rt;
			tempbit |= (valCodeImg << 16);
			valCodeImg = codeImg[i]->word.binI->immed;
			if(codeImg[i]->word.binI->immed >0)
				tempbit |= (valCodeImg);
			else
				tempbit |= (valCodeImg>>16);
		}
		else if (codeImg[i]->type == 'J')
		{
			valCodeImg = codeImg[i]->word.binJ->opcode; 
			tempbit |= (valCodeImg << 26); 
			valCodeImg = codeImg[i]->word.binJ->reg;
			tempbit |= (valCodeImg << 25);
			valCodeImg = codeImg[i]->word.binJ->address;
			tempbit |= (valCodeImg);
		}
		/* Write into the file */
		fprintf(file_desc,"%4d ",i);
		for(j=0;j<4;j++)
		{
			fprintf(file_desc,"%X",(tempbit & 0xF0) >> 4);
			fprintf(file_desc, "%X ",tempbit & 0x0F); 
			tempbit >>= 8;
		}
		fprintf(file_desc,"\n");
	}

	/* Write data image */
	i = 1;
	while (dataImgNode != NULL)
	{
		if(dataImgNode->bytes == DB_BYTES || dataImgNode->bytes == 0)
		{
			/* Write into the file */
			if(i % BYTES_IN_LINE == 1)
				fprintf(file_desc,"%4d ",icf+i-1);
			valDataImg = (dataImgNode->dataBin).bin8->number; 
			fprintf(file_desc,"%X",(valDataImg & 0xF0) >> 4);
			fprintf(file_desc,"%X ",valDataImg & 0x0F);
			if(i % BYTES_IN_LINE == 0 && i!=0)
				fprintf(file_desc,"\n");
			i++;
		}
		if(dataImgNode->bytes == DH_BYTES)
			valDataImg = (dataImgNode->dataBin).bin16->number; 
		if(dataImgNode->bytes == DW_BYTES)
			valDataImg = (dataImgNode->dataBin).bin32->number; 
		if(dataImgNode->bytes == DH_BYTES|| dataImgNode->bytes == DW_BYTES)
		{
			tempbit |= (valDataImg<<(dataImgNode->bytes*2)); 
			for(j=0;j<dataImgNode->bytes;j++)
			{
				/* Write into the file */
				if(i% BYTES_IN_LINE == 1)
					fprintf(file_desc,"%4d ",icf+i-1);
				fprintf(file_desc,"%X",(tempbit & 0xF0) >> 4);
				fprintf(file_desc, "%X ",tempbit & 0x0F);
				tempbit >>= dataImgNode->bytes*2;
				if(i% BYTES_IN_LINE == 0 && i!=0)
					fprintf(file_desc,"\n");
				i++;
			}
		}
		dataImgNode = dataImgNode->next;
	}
	/* Close the file */
	fclose(file_desc);
	return TRUE;
}
