#ifndef GLOBALS_H
#define GLOBALS_H
typedef enum booleans
{
	FALSE = 0, TRUE = 1
} bool;

#define FIRST_IC 100
#define MAX_LINE 80 
#define NONE_REG -1

#define DB_BYTES 1
#define DH_BYTES 2
#define DW_BYTES 4
#define CODE_ARR_IMG_LENGTH 1500
typedef struct line {
	/* Line number */
	int number;
	/* Line content */
	char *text;
} line;

typedef enum opcode {
	/* R Group */
	ADD_OP = 0,
	SUB_OP = 0,
	AND_OP = 0,
	OR_OP = 0,
	NOR_OP = 0,
	MOVE_OP = 1,
	MVHI_OP = 1,
	MVLO_OP = 1,

	/*I GROUP*/
	ADDI_OP = 10,
	SUBI_OP = 11,
	ANDI_OP = 12,
	ORI_OP = 13,
	NORI_OP = 14,
	BNE_OP = 15,
	BEQ_OP = 16,
	BLT_OP = 17,
	BGT_OP = 18,
	LB_OP = 19,
	SB_OP = 20,
	LW_OP = 21,
	SW_OP = 22,
	LH_OP = 23,
	SH_OP = 24,

	/*J GROUP*/
	JMP_OP = 30,
	LA_OP = 31,
	CALL_OP = 32,
	STOP_OP = 63,
	
	/* Failed/Error */
	NONE_OP = -1
} opcode;

/* by func */
typedef enum funct {
	/* OPCODE 0 */
	ADD_FUNCT = 1,
	SUB_FUNCT = 2,
	AND_FUNCT = 3,
	OR_FUNCT = 4,
	NOR_FUNCT = 5,

	/* OPCODE 1 */
	MOVE_FUNCT = 1,
	MVHI_FUNCT = 2,
	MVLO_FUNCT = 3,

	/** Default (No need/Error) */
	NONE_FUNCT = 0
} funct;

typedef enum guide
{
	DB_GUIDE = 1,
	DH_GUIDE = 2,
	DW_GUIDE = 3,
	ASCIZ_GUIDE = 4,
	ENTRY_GUIDE = 5,
	EXTERN_GUIDE = 6,
	ERROR_GUIDE = -1,
	NONE_GUIDE = 0	
}guide;

struct symbolNode
{
	char symbol[MAX_LINE];	
	int value;
	char* attribute;
    struct symbolNode* next;
}symbolNode;




#endif

