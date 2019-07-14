#include <stdlib.h>
#include "semantic.h"

int semanticError = 0;
int currentFunctionType = 0;

int isArithmeticExpression(AST* node) {
  return
    node->type == AST_EXP_SUM ||
    node->type == AST_EXP_SUB ||
    node->type == AST_EXP_MUL ||
    node->type == AST_EXP_DIV ||
    node->type == AST_EXP_MOD;
}

int isPowerExpression(AST* node) {
  return node->type == AST_EXP_POW;
}

int isRelationalExpression(AST* node) {
  return
    node->type == AST_EXP_LESS ||
    node->type == AST_EXP_GREATER ||
    node->type == AST_EXP_LE ||
    node->type == AST_EXP_GE ||
    node->type == AST_EXP_EQ ||
    node->type == AST_EXP_DIF;
}

int isLogicalExpression(AST* node) {
  return
    node->type == AST_EXP_AND ||
    node->type == AST_EXP_NOT ||
    node->type == AST_EXP_OR;
}

int isLiteral(AST* node) {
  return
    node->type == AST_LIT_INT ||
    node->type == AST_LIT_CHAR ||
    node->type == AST_LIT_FLOAT;
}

int literalType(AST* node) {
  switch(node->type) {
    case AST_LIT_INT:
    case AST_LIT_CHAR:
      return DATATYPE_INTEGER;
    case AST_LIT_FLOAT:
      return DATATYPE_FLOAT;
  }

  int val =
    node->type == AST_LIT_INT ||
    node->type == AST_LIT_CHAR ||
    node->type == AST_LIT_FLOAT;
  return val;
}

int isOperation(AST* node) {
  return
    isLogicalExpression(node) ||
    isArithmeticExpression(node) ||
    isRelationalExpression(node) ||
    isPowerExpression(node);
}

int getType(AST* node) {
  if (isOperation(node)) return node->datatype;
  if (isLiteral(node)) return literalType(node);

  if (node->type == AST_IDENTIFIER) {
    if (node->symbol->type == SYMBOL_VECTOR) {
      fprintf(stderr, "[SEMANTIC ERROR] Vector variable without index. Line: %d\n", node->lineNum);
      semanticError++;
    }
    return node->symbol->datatype;
  }

  if (node->type == AST_EXP_PARENTHESIS || node->type == AST_FUNC_CALL) {
    return getType(node->son[0]);
  }

  if (node->type == AST_VECTOR_ACCS) {
    return node->son[0]->symbol->datatype;
  }

  return 0;
}

void checkArithmeticExpression(AST* node) {
  if (!isArithmeticExpression(node)) return;

  int leftHandSideType = getType(node->son[0]);
  int rightHandSideType = getType(node->son[1]);

  int wrongType =
    leftHandSideType == DATATYPE_BOOL ||
    rightHandSideType == DATATYPE_BOOL ||
    leftHandSideType == 0 ||
    rightHandSideType == 0;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Arithmetic expression should not have booleans. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }

  if (node->type == AST_EXP_DIV) {
    node->datatype = DATATYPE_FLOAT;
    return;
  }

  if (leftHandSideType > rightHandSideType) {
    node->datatype = leftHandSideType;
  } else {
    node->datatype = rightHandSideType;
  }
}

void checkLogicalExpression(AST* node) {
  if (!isLogicalExpression(node)) return;

  int leftHandSideType = getType(node->son[0]);
  int rightHandSideType = getType(node->son[1]);

  int wrongType =
    leftHandSideType != DATATYPE_BOOL ||
    (rightHandSideType != DATATYPE_BOOL &&
    node->type != AST_EXP_NOT);

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Logical expression with wrong types. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }

  node->datatype = DATATYPE_BOOL;
}

void checkPowerExpression(AST* node) {
  if (!isPowerExpression(node)) return;

  int leftHandSideType = getType(node->son[0]);
  int rightHandSideType = getType(node->son[1]);

  int wrongType =
    leftHandSideType != DATATYPE_INTEGER ||
    rightHandSideType != DATATYPE_INTEGER;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Power expression with wrong types, should be int or byte. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }

  node->datatype = DATATYPE_INTEGER;
}

void checkRelationalExpression(AST* node) {
  if (!isRelationalExpression(node)) return;

  int leftHandSideType = getType(node->son[0]);
  int rightHandSideType = getType(node->son[1]);

  int wrongType =
    leftHandSideType == DATATYPE_BOOL ||
    rightHandSideType == DATATYPE_BOOL ||
    leftHandSideType == 0 ||
    rightHandSideType == 0;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Relational expression with wrong types. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }

  node->datatype = DATATYPE_BOOL;
}

void checkAssignment(AST* node) {
  if (node->type != AST_ASSIGN || node->type != AST_VEC_ASSIGN) return;

  int leftHandSideType = getType(node->son[0]);
  int rightHandSideType = getType(node->son[1]);

  int wrongType = leftHandSideType != rightHandSideType;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Assigning wrong type. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }
}

void checkFunctionCall(AST* node) {
  if (node->type != AST_FUNC_CALL) return;

  AST* args = node->son[1]->son[0];
  PARAM* params = node->son[0]->symbol->params;

  while(args && params && getType(args->son[0]) == params->symbol->datatype) {
    params = params->next;
    args = args->son[1];
  }

  int wrongType = args || params;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Function call with wrong argument. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }
}

void checkVectorAcess(AST* node) {
  if (node->type != AST_VECTOR_ACCS) return;

  int wrongType = getType(node->son[1]) != DATATYPE_INTEGER;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Vector acess with non natural number type. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }
}

void registerFunctionContext(AST* node) {
  if (node->type != AST_FUNC) return;
  currentFunctionType = getType(node->son[1]);
}

void checkReturns(AST* node) {
  if (node->type != AST_RETURN) return;

  int wrongType = getType(node->son[0]) != currentFunctionType;

  if (wrongType) {
    fprintf(stderr, "[SEMANTIC ERROR] Return type mismatch function type. Line: %d\n", node->lineNum);
    semanticError++;
    return;
  }
}

void checkUndeclared(AST *node) {
  if (node->type == AST_IDENTIFIER && node->symbol->datatype == 0) {
    fprintf(stderr, "[SEMANTIC ERROR] Identifier %s is undeclared. Line: %d\n", node->symbol->text, node->lineNum);
    semanticError++;
  }
}

void typeCheck(AST *node) {
  if (node == NULL) return;

  registerFunctionContext(node);

  typeCheck(node->son[0]);
  typeCheck(node->son[1]);
  typeCheck(node->son[2]);
  typeCheck(node->son[3]);

  checkUndeclared(node);
  checkArithmeticExpression(node);
  checkRelationalExpression(node);
  checkLogicalExpression(node);
  checkPowerExpression(node);
  checkAssignment(node);
  checkFunctionCall(node);
  checkVectorAcess(node);
  checkReturns(node);
}

void setAndCheckDeclaration(AST *node) {
  HASH_NODE* symbol = node->son[0]->son[1]->symbol;
  AST* declaration = node->son[0];

  int datatype = node->son[0]->son[0]->type;

  if (datatype == AST_INT || datatype == AST_BYTE) {
    symbol->datatype = DATATYPE_INTEGER;
  } else {
    symbol->datatype = DATATYPE_FLOAT;
  }

  if (symbol->type != SYMBOL_TK_IDENTIFIER) {
    fprintf(stderr, "[SEMANTIC ERROR] Identifier %s redeclared. Line: %d\n", symbol->text, node->lineNum);
    semanticError++;
  } else {
    switch (declaration->type) {
      case AST_VARIABLE:
        symbol->type = SYMBOL_VAR;
        break;

      case AST_VECTOR:
        symbol->type = SYMBOL_VECTOR;

        if (declaration->son[2]->type != AST_LIT_INT) {
          fprintf(stderr, "[SEMANTIC ERROR] Vector %s indexed with wrong type. Line: %d\n", symbol->text, node->lineNum);
          semanticError++;
        }

        if (declaration->son[3]) {
          AST* initElement;
          int typeError = 0;

          for (initElement = declaration->son[3]->son[0]; initElement != NULL; initElement = initElement->son[1]) {
            int wrongType =
              ((initElement->son[0]->type == AST_LIT_INT || initElement->son[0]->type == AST_LIT_CHAR) &&
              symbol->datatype != DATATYPE_INTEGER) ||
              (initElement->son[0]->type == AST_LIT_FLOAT &&
              symbol->datatype != DATATYPE_FLOAT);

            if (wrongType) { typeError++; }
          }
          if (typeError) {
            fprintf(stderr, "[SEMANTIC ERROR] Vector %s initialized with wrong type. Line: %d\n", symbol->text, node->lineNum);
            semanticError++;
          }
        }
        break;

      case AST_FUNC:
        symbol->type = SYMBOL_FUNCTION;
        AST* parameters = declaration->son[2]->son[0];

        if (parameters) {
          AST* param;
          for (param = parameters; param != NULL; param = param->son[1]) {

            PARAM* newParam = (PARAM*) calloc(1, sizeof(PARAM));
            newParam->symbol = param->son[0]->son[1]->symbol;
            newParam->symbol->type = SYMBOL_VAR;

            if (param->son[0]->son[0]->type == AST_INT || param->son[0]->son[0]->type == AST_BYTE) {
              newParam->symbol->datatype = DATATYPE_INTEGER;
            } else {
              newParam->symbol->datatype = DATATYPE_FLOAT;
            }

            newParam->next = NULL;

            PARAM* params = symbol->params;
            if(params) {
              while (params->next) {
                params = params->next;
              }
              params->next = newParam;
            } else {
              symbol->params = newParam;
            }
          }
        }
        break;
    }
  }
}

void setAndCheckDeclarations(AST *node) {
  if (node == NULL) return;

  if (node->type == AST_DECL) {
    setAndCheckDeclaration(node);
  }

  setAndCheckDeclarations(node->son[0]);
  setAndCheckDeclarations(node->son[1]);
  setAndCheckDeclarations(node->son[2]);
  setAndCheckDeclarations(node->son[3]);
}

int semanticCheck(AST *node) {
  setAndCheckDeclarations(node);
  typeCheck(node);

  if (semanticError) {
    fprintf(stderr, "%d semantic errors found!\n", semanticError);
    exit(4);
  }
  return 0;
}
