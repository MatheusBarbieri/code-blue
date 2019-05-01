#include <stdlib.h>
#include "ast.h"

AST* astCreate(int type, struct hashNode *symbol, AST *node0, AST *node1, AST *node2, AST *node3){
	AST* newnode;
	newnode = (AST*) calloc(1,sizeof(AST));
	newnode->type = type;
	newnode->symbol = symbol;
	newnode->son[NODE_ZERO] = node0;
	newnode->son[NODE_ONE]   = node1;
	newnode->son[NODE_TWO] = node2;
	newnode->son[NODE_THREE] = node3;
	return newnode;
}

void astPrint(int level, AST *node){
	int i = 0;

	if(node == 0)
		return;

	for(i=0; i<level; i++)
		fprintf(stderr, "    ");

	switch(node->type) {
		case AST_DECL: fprintf(stderr, "AST_DECL, "); break;
		case AST_VARIABLE: fprintf(stderr, "AST_VARIABLE, "); break;
		case AST_VECTOR: fprintf(stderr, "AST_VECTOR, "); break;
		case AST_VECTOR_INIT: fprintf(stderr, "AST_VECTOR_INIT, "); break;
		case AST_VECTOR_ACCS: fprintf(stderr, "AST_VECTOR_ACCS, "); break;
		case AST_BYTE: fprintf(stderr, "AST_BYTE, "); break;
		case AST_INT: fprintf(stderr, "AST_INT, "); break;
		case AST_FLOAT: fprintf(stderr, "AST_FLOAT, "); break;
		case AST_LIT_INT: fprintf(stderr, "AST_LIT_INT, "); break;
		case AST_LIT_FLOAT: fprintf(stderr, "AST_LIT_FLOAT, "); break;
		case AST_LIT_CHAR: fprintf(stderr, "AST_LIT_CHAR, "); break;
		case AST_LIT_LIST: fprintf(stderr, "AST_LIT_LIST, "); break;
		case AST_FUNC: fprintf(stderr, "AST_FUNC, "); break;
		case AST_FUNC_PARAMS: fprintf(stderr, "AST_FUNC_PARAMS, "); break;
		case AST_FUNC_CALL: fprintf(stderr, "AST_FUNC_CALL, "); break;
		case AST_FUNC_ARGS: fprintf(stderr, "AST_FUNC_ARGS, "); break;
		case AST_ARG_LIST: fprintf(stderr, "AST_ARG_LIST, "); break;
		case AST_PARAM_LIST: fprintf(stderr, "AST_PARAM_LIST, "); break;
		case AST_PARAM: fprintf(stderr, "AST_PARAM, "); break;
		case AST_CMD_BLOCK: fprintf(stderr, "AST_CMD_BLOCK, "); break;
		case AST_CMD_LIST: fprintf(stderr, "AST_CMD_LIST, "); break;
		case AST_CMD: fprintf(stderr, "AST_CMD, "); break;
		case AST_ASSING: fprintf(stderr, "AST_ASSING, "); break;
		case AST_LEAP: fprintf(stderr, "AST_LEAP, "); break;
		case AST_INPUT: fprintf(stderr, "AST_INPUT, "); break;
		case AST_PRINT: fprintf(stderr, "AST_PRINT, "); break;
		case AST_PRINT_LIST: fprintf(stderr, "AST_PRINT_LIST, "); break;
		case AST_LIT_STRING: fprintf(stderr, "AST_LIT_STRING, "); break;
		case AST_PRINT_EXP: fprintf(stderr, "AST_PRINT_EXP, "); break;
		case AST_RETURN: fprintf(stderr, "AST_RETURN, "); break;
		case AST_IF: fprintf(stderr, "AST_IF, "); break;
		case AST_LOOP: fprintf(stderr, "AST_LOOP, "); break;
		case AST_EXP_PARENTHESIS: fprintf(stderr, "AST_EXP_PARENTHESIS, "); break;
		case AST_EXP_LIT: fprintf(stderr, "AST_EXP_LIT, "); break;
		case AST_EXP_FUNC: fprintf(stderr, "AST_EXP_FUNC, "); break;
		case AST_EXP_IDENTIFIER: fprintf(stderr, "AST_EXP_IDENTIFIER, "); break;
		case AST_EXP_VECTOR_ACCS: fprintf(stderr, "AST_EXP_VECTOR_ACCS, "); break;
		case AST_EXP_SUM: fprintf(stderr, "AST_EXP_SUM, "); break;
		case AST_EXP_SUB: fprintf(stderr, "AST_EXP_SUB, "); break;
		case AST_EXP_MUL: fprintf(stderr, "AST_EXP_MUL, "); break;
		case AST_EXP_DIV: fprintf(stderr, "AST_EXP_DIV, "); break;
		case AST_EXP_MOD: fprintf(stderr, "AST_EXP_MOD, "); break;
		case AST_EXP_POW: fprintf(stderr, "AST_EXP_POW, "); break;
		case AST_EXP_LESS: fprintf(stderr, "AST_EXP_LESS, "); break;
		case AST_EXP_GREATER: fprintf(stderr, "AST_EXP_GREATER, "); break;
		case AST_EXP_LE: fprintf(stderr, "AST_EXP_LE, "); break;
		case AST_EXP_GE: fprintf(stderr, "AST_EXP_GE, "); break;
		case AST_EXP_EQ: fprintf(stderr, "AST_EXP_EQ, "); break;
		case AST_EXP_DIF: fprintf(stderr, "AST_EXP_DIF, "); break;
		case AST_EXP_AND: fprintf(stderr, "AST_EXP_AND, "); break;
		case AST_EXP_OR: fprintf(stderr, "AST_EXP_OR, "); break;
		case AST_EXP_NOT: fprintf(stderr, "AST_EXP_NOT, "); break;

		default: fprintf(stderr, "Not implemented. ");
	}

	if(node->symbol != 0)
		fprintf(stderr, "%s, \n", node->symbol->text);
	else
		fprintf(stderr, "<no symbol>, \n");

	for (i=0; i<MAX_NODES; i++)
		astPrint(level+1, node->son[i]);
}

/* int main() {

struct hashNode *node1 = hashInsert(1, "cristiano");
struct hashNode *node2 = hashInsert(1, "salla");
struct hashNode *node3 = hashInsert(1, "lalala");
struct hashNode *node4 = hashInsert(1, "lunardi");
struct hashNode *node5 = hashInsert(1, "teste");
struct hashNode *node6 = hashInsert(1, "stringa");
struct hashNode *node7 = hashInsert(1, "inteiro");
//hashPrint();



AST *tree7 = astCreate(1, node7, 0, 0 ,0, 0);
AST *tree6 = astCreate(1, node6, 0, 0 ,0, 0);
AST *tree5 = astCreate(1, node5, 0, 0 ,0, 0);
AST *tree4 = astCreate(1, node4, 0, 0,0, 0);
AST *tree3 = astCreate(1, node3, tree6, tree7 ,0, 0);
AST *tree2 = astCreate(1, node2, tree4, tree5 ,0, 0);
AST *tree1 = astCreate(1, node1, tree2, tree3, 0, 0);

astPrint(0, tree1);
//printf("(Table[0] = %s)\n", Table[97]->text);
return -1;
} */
