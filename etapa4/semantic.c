#include "semantic.h"

#define AST_VARDEC 1
#define AST_FUNDEC 2

void setAndCheckRedeclared(AST *node) {
  if (node == 0)
    return;

  for (int i= 0; i< MAX_NODES; i++)
    setAndCheckRedeclared(node->son[i]);

  switch (node->type) {
    case AST_VARDEC:
    if (node->symbol->type == SYMBOL_IDENTIFIER) {
      node->symbol->type = SYMBOL_VAR;
    } else {
      fprintf(stderr, "SEMANTIC ERROR: Identifier %s redeclared\n", node->symbol->text);
    }
    break;
    case AST_FUNDEC:
    if (node->symbol->type == SYMBOL_IDENTIFIER) {
      node->symbol->type = SYMBOL_FUN;
    } else {
      fprintf(stderr, "SEMANTIC ERROR: Identifier %s redeclared\n", node->symbol->text);
    }
    break;
  }

}
