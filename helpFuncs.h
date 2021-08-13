#ifndef HELP_FUNCS_H
#define HELP_FUNCS_H
#include "globals.h"
#define SKIP_WHITE_SPACE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;
int printError(int lineNum, char* error, ...);
char *mallocName(char *s0);
void *check_malloc(long size);
bool have_label(line line , char* label);
bool valid_label(char *a_label);
bool check_digit_alpha(char *str);
bool is_label(line line , char* label);
bool check_reserved_word(char *a_label);
bool isComment(line line, int i);
void is_register(char *a_label, int *reg);
bool isInt(char *str);
#endif



