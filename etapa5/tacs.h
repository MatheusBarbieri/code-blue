#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL      1

#define TAC_ADD 		2
#define TAC_SUB 		3
#define TAC_DIV 		4
#define TAC_MUL 		5

#define TAC_LT 			6
#define TAC_GT 			7
#define TAC_LE 			8
#define TAC_GE 		    9
#define TAC_EQ 			10
#define TAC_OR 			11
#define TAC_AND 		12

#define TAC_BEGINFUN 	13
#define TAC_ENDFUN 		14
#define TAC_RETURN 		15

#define TAC_IFZ         16
#define TAC_JUMP 		17

#define TAC_CALL 		18
#define TAC_ARG 		19

#define TAC_PRINT 		20
#define TAC_READ 		21

#define TAC_LABEL       22

#define TAC_MOVE        23

#define TAC_WHILE        24
#define TAC_VEC_WR        25
#define TAC_VEC_RD        26
#define TAC_BUFFER        27
#define TAC_FUNCALL        28

#define TAC_PARAM_LIST        29
#define TAC_ARG_LIST        30


typedef struct tac_struct{
    int type;
    HASH_NODE* res;
    HASH_NODE* op1;
    HASH_NODE* op2;
    struct tac_struct * prev;
    struct tac_struct * next;
} TAC;

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
void tacPrintSingle(TAC* tac);
void tacPrintBackwards(TAC* tac);
void tacPrintForward(TAC* tac);
TAC* tacReverse(TAC* tac);

// CODE GENERATION

TAC* tacGenerate(AST* node);
TAC* tacJoin(TAC* l1, TAC* l2);

#endif
