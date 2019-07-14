#ifndef TACS_HEADER
#define TACS_HEADER

#include <stdlib.h>
#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 1
#define TAC_LABEL 2
#define TAC_ADD 3
#define TAC_SUB 4
#define TAC_DIV 5
#define TAC_MUL 6
#define TAC_LT 7
#define TAC_GT 8
#define TAC_LE 9
#define TAC_GE 10
#define TAC_EQ 11
#define TAC_OR 12
#define TAC_AND 13
#define TAC_DIF 14
#define TAC_NOT 15
#define TAC_BEGINFUN 16
#define TAC_ENDFUN 17
#define TAC_PARAM 18
#define TAC_RETURN 19
#define TAC_IFZ 20
#define TAC_JUMP 21
#define TAC_FUNCALL 22
#define TAC_ARG_PUSH 23
#define TAC_POP 24
#define TAC_PRINT 25
#define TAC_READ 26
#define TAC_MOVE 27
#define TAC_VEC 28
#define TAC_VEC_INIT 29
#define TAC_VEC_MOVE 30
#define TAC_VEC_ACC 31

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
TAC* indexReverseOrder(TAC* tac);
TAC* tacGenerate(AST* node, HASH_NODE* loopLabel);
TAC* tacJoin(TAC* l1, TAC* l2);

#endif
