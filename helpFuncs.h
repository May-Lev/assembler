/* Helper functions to process and analyze code */
#ifndef HELP_FUNCS_H
#define HELP_FUNCS_H
#include "globals.h"
#define SKIP_WHITE_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;
/* The function shows error message to the user, with line number and an optional values. Returns false.  */
bool printError(int lineNum, char* error, ...);
/* The function checks if the filename is ok, with .as extension */
char *checkFileName(char *str);
/* The function adds extension to file name (used in the making of output files) */
char *addExtensionToFile(char *filename, char *fileExtension);
/* The function checks if malloc succeed */
void *check_malloc(long size);
/* The function checks length and alphabet of the label name, and if it's a saved word (instruct name for example) */
bool valid_label(char *label);
/*check for every char in string if it is non alphanumeric char if it is function returns true*/   
bool checkDigitAlpha(char *str);
/* The function checks if line has (a valid) label , and updates the label char* to contain it */
bool is_label(line line , char* label);
/* The function checks if the label is a saved word (instruct name for example) */
bool savedWord(char *label);
/* The function checks if the line in empty or a comment */
bool isComment(line line, int i);
/* The function checks if the value is an integer */
bool isInt(char *str);
/* The A function checks the number of operands, and if the commas are in the right places */
bool operandsCheck(line line, int i, char **operands, int *operandCount, char *instructName);
#endif



