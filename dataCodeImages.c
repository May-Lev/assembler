#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "dataCodeImages.h"
#include "globals.h"



void addToDataImg(int *DC, char* line, int bytes,struct dataImage** dataImg)
{
	int i;
	struct dataImage* new_node = (struct dataImage*) malloc(sizeof(struct dataImage));
    	struct dataImage *last = *dataImg;
	for(i = 0;i<strlen(line);i++)
		new_node->lineText[i] = (char)line[i];
	new_node->bin8 = 0;
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
void addToCodeImg(int *IC, char* line)
{
	printf("rrrrr %d",*IC);
	*IC+=4;
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
		printf(" %d ", currNode->bin8);
		currNode = currNode->next;
	}
}

