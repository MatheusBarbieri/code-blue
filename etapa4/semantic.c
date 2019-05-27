#include <stdlib.h>
#include "semantic.h"

int semanticCheck(AST *node) {
  setAndCheckDeclarations(node);
  return 0;
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
  } else {
    switch (declaration->type) {
      case AST_VARIABLE:
        symbol->type = SYMBOL_VAR;
        break;

      case AST_VECTOR:
        symbol->type = SYMBOL_VECTOR;

        if (declaration->son[2]->type != AST_LIT_INT) {
          fprintf(stderr, "[SEMANTIC ERROR] Vector %s indexed with wrong type.\n", symbol->text);
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

void setAndCheckDeclarations(AST *node) {
  AST* current;

  for (current = node; current != NULL; current = current->son[1]) {
		setAndCheckDeclaration(current);
  }
}
