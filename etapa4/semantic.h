#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include "ast.h"
#include "hash.h"

int semanticCheck(AST *node);
void setAndCheckDeclarations(AST *node);

#endif
