#include <stdio.h>

#define NODE_ZERO 0
#define NODE_ONE 1
#define NODE_TWO 2
#define NODE_THREE 3

#define MAX_NODES 4

#define AST_SYMBOL 1
#define AST_DEC 2

typedef struct ast_node{
    int type;
    struct hashNode *symbol;
    struct ast_node *son[MAX_NODES];
} AST;


AST* astCreate(int type, struct hashNode *symbol, AST *node0, AST *node1, AST *node2, AST *node3);
void astToFile(AST* node, FILE *fileTree);
void astPrint(int level, AST* node);
