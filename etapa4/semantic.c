#include <stdlib.h>
#include "semantic.h"

int semanticError = 0;

int semanticCheck(AST *node) {
  setAndCheckDeclarations(node);

  if (semanticError) {
    fprintf(stderr, "%d semantic errors found!\n", semanticError);
    exit(4);
  }
  return 0;
}

void setAndCheckDeclarations(AST *node) {
  if (node == NULL) return;

  if (node->type == AST_DECL) {
    setAndCheckDeclaration(node);
  }

  if (node->type == AST_VARIABLE) {
    printf("verificar se ja foi declarada\n");
    if (node->son[1]->type == SYMBOL_VAR) {
      printf("verificar se declaração válida\n");
    } else if(node->son[1]->type == AST_EXP_SUM
      || node->son[1]->type == AST_EXP_SUB
      || node->son[1]->type == AST_EXP_MUL
      || node->son[1]->type == AST_EXP_DIV
      || node->son[1]->type == AST_EXP_MOD
      || node->son[1]->type == AST_EXP_POW
      || node->son[1]->type == AST_EXP_LESS
      || node->son[1]->type == AST_EXP_GREATER
      || node->son[1]->type == AST_EXP_LE
      || node->son[1]->type == AST_EXP_GE
      || node->son[1]->type == AST_EXP_EQ
      || node->son[1]->type == AST_EXP_DIF
      || node->son[1]->type == AST_EXP_AND
      || node->son[1]->type == AST_EXP_OR
      || node->son[1]->type == AST_EXP_NOT) {
        checkOperands(node->symbol->datatype, node->son[1]);
      }
    }

  setAndCheckDeclarations(node->son[0]);
  setAndCheckDeclarations(node->son[1]);
  setAndCheckDeclarations(node->son[2]);
  setAndCheckDeclarations(node->son[3]);

  if (node->type == AST_IDENTIFIER && node->symbol->datatype == 0) {
    fprintf(stderr, "[SEMANTIC ERROR] Identifier %s is undeclared\n", node->symbol->text);
    semanticError++;
  }
}

void setAndCheckDeclaration(AST *node) {
  struct hashNode* symbol = node->son[0]->son[1]->symbol;
  AST* declaration = node->son[0];

  int datatype = node->son[0]->son[0]->type;

  if (datatype == AST_INT || datatype == AST_BYTE) {
    symbol->datatype = DATATYPE_INTEGER;
  } else {
    symbol->datatype = DATATYPE_FLOAT;
  }

  if (symbol->type != SYMBOL_TK_IDENTIFIER) {
    fprintf(stderr, "[SEMANTIC ERROR] Identifier %s redeclared\n", symbol->text);
    semanticError++;
  } else {
    switch (declaration->type) {
      case AST_VARIABLE:
        symbol->type = SYMBOL_VAR;
        break;

      case AST_VECTOR:
        symbol->type = SYMBOL_VECTOR;

        if (declaration->son[2]->type != AST_LIT_INT) {
          fprintf(stderr, "[SEMANTIC ERROR] Vector %s indexed with wrong type.\n", symbol->text);
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
            fprintf(stderr, "[SEMANTIC ERROR] Vector %s initialized with wrong type.\n", symbol->text);
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

            struct paramList* newParam = (struct paramList*) calloc(1, sizeof(struct paramList));
            newParam->symbol = param->son[0]->son[1]->symbol;
            newParam->symbol->type = SYMBOL_VAR;

            if (param->son[0]->son[0]->type == AST_INT || param->son[0]->son[0]->type == AST_BYTE) {
              newParam->symbol->datatype = DATATYPE_INTEGER;
            } else {
              newParam->symbol->datatype = DATATYPE_FLOAT;
            }

            newParam->next = NULL;

            struct paramList* params = symbol->params;
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

void checkOperands(int datatype, AST *node) {
  int i;

  if (!node) return;

  for (i = 0; i < MAX_NODES; i++) {
    checkOperands(datatype, node->son[i]);
  }

  switch(node->type)
   {
     case SYMBOL_VECTOR:
     case SYMBOL_FUNCTION:
     case SYMBOL_TK_IDENTIFIER:
      printf("ERROR: operand %s not a variable in %d.\n", node->symbol->text, node->symbol->datatype);
      if(node->symbol->datatype == DATATYPE_INTEGER)
        node->datatype = AST_INT;
      else
        node->datatype = AST_FLOAT;
        break;

     case AST_FUNC_CALL:
      node->datatype = node->son[0]->datatype;
      printf("verificar chamada de funcao\n");
      if(node->son[0]->symbol->datatype == DATATYPE_INTEGER)
        node->datatype = AST_INT;
      else
        node->datatype = AST_FLOAT;
        break;

     case AST_EXP_SUM:
     case AST_EXP_SUB:
     case AST_EXP_MUL:
     case AST_EXP_DIV:
     case AST_EXP_MOD:
     case AST_EXP_POW:
      if(node->son[0]->datatype == AST_BYTE) {
       fprintf(stderr, "[SEMANTIC] Cannot use byte in arithmetic expression.\n");
      }
      else if(node->son[0]->datatype != node->son[1]->datatype) {
        fprintf(stderr, "[SEMANTIC] Arithmetic operation cannot mix float and int data.\n");
      } else {
        node->datatype = node->son[0]->datatype;
      }
      break;
     case AST_EXP_NOT:
      if(node->son[0]->datatype != AST_BYTE) {
       fprintf(stderr, "[SEMANTIC] Only byte allowed to exp not.\n");
      }
      node->datatype = AST_BYTE;
        break;
      case AST_ASSING:
        break;
      case AST_EXP_LESS:
      case AST_EXP_GREATER:
      case AST_EXP_LE:
      case AST_EXP_GE:
      case AST_EXP_EQ:
      case AST_EXP_DIF:
      case AST_EXP_AND:
      case AST_EXP_OR:
        if(node->son[0]->datatype != node->son[1]->datatype) {
          fprintf(stderr, "[SEMANTIC] Cannot mix different datatypes in boolean expression.\n");
        }
        node->datatype = AST_BYTE;
        break;
   }

}
