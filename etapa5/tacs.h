#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_SYMBOL 0
#define TAC_MOVE 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_GREAT 6
#define TAC_LESS 7
#define TAC_GE 8
#define TAC_LE 9
#define TAC_EQ 10
#define TAC_DIF 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14
#define TAC_LABEL 15
#define TAC_BEGINFUN 16
#define TAC_ARGPUSH 17
#define TAC_ENDFUN 18
#define TAC_IFZ 19
#define TAC_JUMP 20
#define TAC_CALL 21
#define TAC_ARG 22
#define TAC_RET 23
#define TAC_PRINT 24
#define TAC_READ 25
#define TAC_VECATTR 26
#define TAC_VEC 27
#define TAC_PARAMPOP 28

typedef struct tac_struct{
    int type;
    HASH_NODE* res;
    HASH_NODE* op1;
    HASH_NODE* op2;
    struct tac_struct * prev;
    struct tac_struct * next;
} TAC;

TAC* newTac(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
TAC* tacJoin(TAC* l1, TAC* l2);
void printAllTacs(TAC* l);
TAC* rewindTac(TAC* l);
void printTac(TAC* l);
TAC* createTacs(AST *node, HASH_NODE *currentLoopLabel);

TAC* createBinop(int type, TAC* sons[]);
TAC* createIf(TAC* sons[]);
TAC* createLoop(TAC* sons[], HASH_NODE *label);
TAC* createLeap(TAC* sons[], HASH_NODE *currentLoopLabel);
TAC* createFunction(TAC* symbol, TAC* params, TAC* code);

#endif
