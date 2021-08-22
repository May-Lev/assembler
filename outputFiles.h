#ifndef _OUTPUTFILES_H
#define __OUTPUTFILES_H
#include "globals.h"
#include "dataCodeImages.h"
#include "symbolTable.h"

/* The function writes the output files of a single assembled file */
bool writeOutputFiles(codeImage** codeImg,dataImage** dataImg, int icf, int dcf, char *filename,exorEnList** externList,exorEnList** enterList);
/* The function writes the external and entry lists into .ext, .ent files */
bool writeExtEntFile(exorEnList** exttorEnrList, char *filename, char *fileExtension);
/* The function writes the code and data image into an .ob file */
bool writeObjFile(codeImage** codeImg,dataImage** dataImg, int icf, int dcf,char *filename,char* fileExtension);
#endif
