#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

int semanticCheck(AST *node);
void setAndCheckDeclarations(AST *node);
void setAndCheckDeclaration(AST *node);
void checkOperands(int datatype, AST *node);
#endif
