/* Helper functions to process and analyze code */
#ifndef _OPCODE_H
#define _OPCODE_H
#include "globals.h"

/**
 *void is_opcode(char *name_op, opcode *is_p, funct *is_fun)
 * @param name_op The command name (string)
 * @param get_op The opcode value destination
 * @param get_fun The funct value destination
 */
void is_opcode(char *name_op, opcode *get_op, funct *get_fun);

#endif
