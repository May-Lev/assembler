#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpFuncs.h"
#include "dataCodeImages.h"
#include "globals.h"






void addToDataImg(int *DC, char* line)
{
	*DC+=1;
}

void addToCodeImg(int *IC, char* line)
{
	*IC+=4;
}
