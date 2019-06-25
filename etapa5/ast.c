#include <stdlib.h>
#include "ast.h"

AST* astCreate(int type, HASH_NODE *symbol, AST *node0, AST *node1, AST *node2, AST *node3, int lineNum){
	AST* newnode;
	newnode = (AST*) calloc(1,sizeof(AST));
	newnode->type = type;
	newnode->symbol = symbol;
	newnode->lineNum = lineNum;
	newnode->son[NODE_ZERO] = node0;
	newnode->son[NODE_ONE] = node1;
	newnode->son[NODE_TWO] = node2;
	newnode->son[NODE_THREE] = node3;
	return newnode;
}

void astPrint(int level, AST *node){
	int i = 0;

	if(node == 0)
		return;

	for(i=0; i<level; i++)
		fprintf(stderr, "  ");

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
		case AST_IDENTIFIER: fprintf(stderr, "AST_IDENTIFIER, "); break;

		default: fprintf(stderr, "Not implemented. ");
	}

	if(node->symbol != 0)
		fprintf(stderr, "\"%s\"\n", node->symbol->text);
	else
		fprintf(stderr, "\n");

	for (i=0; i<MAX_NODES; i++)
		astPrint(level+1, node->son[i]);
}

void astToFile(AST *node, FILE *f){
	if(node == 0)
		return;

	switch(node->type){
		case AST_DECL: {
			astToFile(node->son[NODE_ZERO], f);
			if (node->son[NODE_ONE]) {
				astToFile(node->son[NODE_ONE], f);
			}
			break;
		}
		case AST_VARIABLE: {
			astToFile(node->son[NODE_ZERO], f);
			astToFile(node->son[NODE_ONE], f);
			fprintf(f, " = ");
			astToFile(node->son[NODE_TWO], f);
			fprintf(f, ";\n");
			break;
		}
		case AST_VECTOR: {
			astToFile(node->son[NODE_ZERO], f);
			astToFile(node->son[NODE_ONE], f);
			fprintf(f, " [");
			astToFile(node->son[NODE_TWO], f);
			fprintf(f, "] ");
			if (node->son[NODE_THREE]) {
				astToFile(node->son[NODE_THREE], f);
			}
			fprintf(f, ";\n");
			break;
		}
		case AST_VECTOR_INIT: {
			fprintf(f, ": ");
			astToFile(node->son[NODE_ZERO], f);
			break;
		}
		case AST_VECTOR_ACCS: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " [");
			astToFile(node->son[NODE_ONE], f);
			fprintf(f, "] ");
			break;
		}
		case AST_BYTE: {
			fprintf(f, "byte ");
			break;
		}
		case AST_INT: {
			fprintf(f, "int ");
			break;
		}
		case AST_FLOAT: {
			fprintf(f, "float ");
			break;
		}
		case AST_LIT_INT: {
			fprintf(f, "%s ", node->symbol->text);
			break;
		}
		case AST_LIT_FLOAT: {
			fprintf(f, "%s ", node->symbol->text);
			break;
		}
		case AST_LIT_CHAR: {
			fprintf(f, "'%s' ", node->symbol->text);
			break;
		}
		case AST_LIT_LIST: {
			astToFile(node->son[NODE_ZERO], f);
			if (node->son[NODE_ONE]) {
				fprintf(f, " ");
				astToFile(node->son[NODE_ONE], f);
			}
			break;
		}
		case AST_FUNC: {
			astToFile(node->son[NODE_ZERO], f);
			astToFile(node->son[NODE_ONE], f);
			fprintf(f, " ");
			astToFile(node->son[NODE_TWO], f);
			astToFile(node->son[NODE_THREE], f);
			fprintf(f, ";\n");
			break;
		}
		case AST_FUNC_PARAMS:
		case AST_FUNC_ARGS: {
			fprintf(f, "( ");
			if (node->son[NODE_ZERO]) {
				astToFile(node->son[NODE_ZERO], f);
			}
			fprintf(f, ") ");
			break;
		}
		case AST_FUNC_CALL: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_ARG_LIST:
		case AST_PARAM_LIST:
		case AST_PRINT_LIST: {
			astToFile(node->son[NODE_ZERO], f);
			if (node->son[NODE_ONE]) {
				fprintf(f, ", ");
				astToFile(node->son[NODE_ONE], f);
			}
			break;
		}
		case AST_PARAM: {
			astToFile(node->son[NODE_ZERO], f);
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_CMD_BLOCK: {
			fprintf(f, "{\n");
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, "}");
			break;
		}
		case AST_CMD_LIST: {
			astToFile(node->son[NODE_ZERO], f);
			if (node->son[NODE_ONE]) {
				fprintf(f, ";\n");
				astToFile(node->son[NODE_ONE], f);
			}
			break;
		}
		case AST_CMD: {
			if (node->son[NODE_ZERO]) {
				fprintf(f, " ");
				astToFile(node->son[NODE_ZERO], f);
				fprintf(f, " ");
			}
			break;
		}
		case AST_ASSING: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " = ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_LEAP: {
			fprintf(f, "leap ");
			break;
		}
		case AST_INPUT: {
			fprintf(f, "read ");
			astToFile(node->son[NODE_ZERO], f);
			break;
		}
		case AST_PRINT: {
			fprintf(f, "print ");
			astToFile(node->son[NODE_ZERO], f);
			break;
		}
		case AST_LIT_STRING: {
			fprintf(f, "\"%s\"", node->symbol->text);
			break;
		}
		case AST_PRINT_EXP: {
			astToFile(node->son[NODE_ZERO], f);
			break;
		}
		case AST_RETURN: {
			fprintf(f, "return ");
			astToFile(node->son[NODE_ZERO], f);
			break;
		}
		case AST_IF: {
			fprintf(f, "if (");
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, ") then ");
			astToFile(node->son[NODE_ONE], f);
			if (node->son[NODE_TWO]) {
				fprintf(f, " else ");
				astToFile(node->son[NODE_TWO], f);
			}
			break;
		}
		case AST_LOOP: {
			fprintf(f, "loop ( ");
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, ") ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_PARENTHESIS: {
			fprintf(f, "( ");
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, ") ");
			break;
		}
		case AST_EXP_SUM: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " + ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_SUB: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " - ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_MUL: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " * ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_DIV: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " / ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_MOD: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " %% ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_POW: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " ^ ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_LESS: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " < ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_GREATER: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " > ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_LE: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " <= ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_GE: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " >= ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_EQ: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " == ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_DIF: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " != ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_AND: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " and ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_OR: {
			astToFile(node->son[NODE_ZERO], f);
			fprintf(f, " or ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_EXP_NOT: {
			fprintf(f, " not ");
			astToFile(node->son[NODE_ONE], f);
			break;
		}
		case AST_IDENTIFIER: {
			fprintf(f, "%s", node->symbol->text);
			break;
		}
		default: break;
	}
}
