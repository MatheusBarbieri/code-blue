#ifndef AST_HEADER
#define AST_HEADER

#include <stdio.h>
#include "hash.h"

#define NODE_ZERO 0
#define NODE_ONE 1
#define NODE_TWO 2
#define NODE_THREE 3

#define MAX_NODES 4

#define AST_DECL 2
#define AST_VARIABLE 3
#define AST_VECTOR 4
#define AST_VECTOR_INIT 5
#define AST_VECTOR_ACCS 6
#define AST_BYTE 7
#define AST_INT 8
#define AST_FLOAT 9
#define AST_LIT_INT 10
#define AST_LIT_FLOAT 11
#define AST_LIT_CHAR 12
#define AST_LIT_LIST 13
#define AST_FUNC 14
#define AST_FUNC_PARAMS 15
#define AST_FUNC_CALL 16
#define AST_FUNC_ARGS 17
#define AST_ARG_LIST 18
#define AST_PARAM_LIST 19
#define AST_PARAM 20
#define AST_CMD_BLOCK 21
#define AST_CMD_LIST 22
#define AST_CMD 23
#define AST_ASSING 24
#define AST_LEAP 25
#define AST_INPUT 26
#define AST_PRINT 27
#define AST_PRINT_LIST 28
#define AST_LIT_STRING 29
#define AST_PRINT_EXP 30
#define AST_RETURN 31

#define AST_IF 32
#define AST_LOOP 33

#define AST_EXP_PARENTHESIS 34
#define AST_EXP_SUM 35
#define AST_EXP_SUB 36
#define AST_EXP_MUL 37
#define AST_EXP_DIV 38
#define AST_EXP_MOD 39
#define AST_EXP_POW 40
#define AST_EXP_LESS 41
#define AST_EXP_GREATER 42
#define AST_EXP_LE 43
#define AST_EXP_GE 44
#define AST_EXP_EQ 45
#define AST_EXP_DIF 46
#define AST_EXP_AND 47
#define AST_EXP_OR 48
#define AST_EXP_NOT 49
#define AST_IDENTIFIER 50

typedef struct astNode{
    int type;
    int datatype;
    HASH_NODE *symbol;
    struct astNode *son[MAX_NODES];
} AST;

AST* astCreate(int type, HASH_NODE *symbol, AST *node0, AST *node1, AST *node2, AST *node3);
void astToFile(AST* node, FILE *fileTree);
void astPrint(int level, AST* node);

#endif
