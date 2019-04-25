#include <stdlib.h>

#include "hash.c"
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
   		fprintf(stderr, "   ");

      switch(node->type) {
    		case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL, "); 			break;
    		case AST_DEC: fprintf(stderr, "AST_DEC, "); 				break;
      }

      	if(node->symbol != 0)
      		fprintf(stderr, "%s, \n", node->symbol->text);
      	else
      		fprintf(stderr, "<no symbol>, \n");

      	for (i=0; i<MAX_NODES; i++)
      		astPrint(level+1, node->son[i]);

}

int main() {

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

printf("meu peru\n");

  return -1;
}
