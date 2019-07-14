#include "tacs.h"

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2) {
  TAC* newtac;

  newtac = (TAC*) calloc(1, sizeof(TAC));
  newtac->type = type;
  newtac->res = res;
  newtac->op1 = op1;
  newtac->op2 = op2;
  newtac->prev = NULL;
  newtac->next = NULL;

  return newtac;
}

void tacPrintSingle(TAC* tac) {
  if(!tac) return;

  if(tac->type == TAC_SYMBOL) return;

  fprintf(stderr, "TAC (");
  switch(tac->type) {
    case TAC_LABEL: fprintf(stderr, "TAC_LABEL"); break;
    case TAC_ADD: fprintf(stderr, "TAC_ADD"); break;
    case TAC_SUB: fprintf(stderr, "TAC_SUB"); break;
    case TAC_DIV: fprintf(stderr, "TAC_DIV"); break;
    case TAC_MUL: fprintf(stderr, "TAC_MUL"); break;
    case TAC_LT: fprintf(stderr, "TAC_LT"); break;
    case TAC_GT: fprintf(stderr, "TAC_GT"); break;
    case TAC_LE: fprintf(stderr, "TAC_LE"); break;
    case TAC_GE: fprintf(stderr, "TAC_GE"); break;
    case TAC_EQ: fprintf(stderr, "TAC_EQ"); break;
    case TAC_OR: fprintf(stderr, "TAC_OR"); break;
    case TAC_AND: fprintf(stderr, "TAC_AND"); break;
    case TAC_DIF: fprintf(stderr, "TAC_DIF"); break;
    case TAC_NOT: fprintf(stderr, "TAC_NOT"); break;
    case TAC_BEGINFUN: fprintf(stderr, "TAC_BEGINFUN"); break;
    case TAC_ENDFUN: fprintf(stderr, "TAC_ENDFUN"); break;
    case TAC_RETURN: fprintf(stderr, "TAC_RETURN"); break;
    case TAC_IFZ: fprintf(stderr, "TAC_IFZ"); break;
    case TAC_JUMP: fprintf(stderr, "TAC_JUMP"); break;
    case TAC_ARG: fprintf(stderr, "TAC_ARG"); break;
    case TAC_PRINT: fprintf(stderr, "TAC_PRINT"); break;
    case TAC_READ: fprintf(stderr, "TAC_READ"); break;
    case TAC_ASS: fprintf(stderr, "TAC_ASS"); break;
    case TAC_VEC: fprintf(stderr, "TAC_VEC"); break;
    case TAC_VEC_INIT: fprintf(stderr, "TAC_VEC_INIT"); break;
    case TAC_VEC_ASS: fprintf(stderr, "TAC_VEC_ASS"); break;
    case TAC_VEC_ACC: fprintf(stderr, "TAC_VEC_ACC"); break;
    case TAC_FUNCALL: fprintf(stderr, "TAC_FUNCALL"); break;
    case TAC_PARAM: fprintf(stderr, "TAC_PARAM"); break;

    default: fprintf(stderr, "TAC_UNKNOWN"); break;
  }

  if(tac->res) fprintf(stderr, ", %s", tac->res->text);
    else fprintf(stderr, ", 0");

  if(tac->op1) fprintf(stderr, ", %s", tac->op1->text);
    else fprintf(stderr, ", 0");

  if(tac->op2) fprintf(stderr, ", %s", tac->op2->text);
    else fprintf(stderr, ", 0");

  fprintf(stderr, ")\n");
}

void tacPrintBackwards(TAC* tac) {
  if(!tac) return;
  tacPrintSingle(tac);
  tacPrintBackwards(tac->prev);
}

TAC* tacJoin(TAC* l1, TAC* l2) {
  if(!l1) return l2;
  if(!l2) return l1;

  TAC* aux;
  for(aux = l2; aux->prev; aux = aux->prev);

  aux->prev = l1;

  return l2;
}

TAC* tacReverse(TAC* tac) {
  if(!tac) return 0;

  for(; tac->prev; tac = tac->prev)
    tac->prev->next = tac;

  return tac;
}

void tacPrintForward(TAC* tac) {
  if(!tac) return;

  tacPrintSingle(tac);
  tacPrintForward(tac->next);
}

////////////////////////////////////////////

TAC* makeBinOp(int type, TAC* left, TAC* right) {
  HASH_NODE* leftRes = left ? left->res : NULL;
  HASH_NODE* rightRes = right ? right->res : NULL;
  return tacJoin(tacJoin(left, right), tacCreate(type, makeTemp(), leftRes, rightRes));
}

TAC* makeLoop(TAC* result0, TAC* result1, HASH_NODE *whileLabel) {
	HASH_NODE* jumpLabel = makeLabel();

	TAC* whileTac = tacCreate(TAC_IFZ, jumpLabel, result0 ? result0->res : 0, 0);
	TAC* whileLabelTac = tacCreate(TAC_LABEL, whileLabel, 0, 0);

	TAC* jumpTac = tacCreate(TAC_JUMP, whileLabel, 0, 0);
	TAC* jumpLabelTac = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

	return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(
      whileLabelTac,
			result0),
			whileTac),
			result1),
			jumpTac),
			jumpLabelTac);
}

TAC* makeLeap(HASH_NODE *loopLabel) {
	return tacCreate(TAC_JUMP, loopLabel, 0, 0);
}

TAC* makeVar(TAC* left, TAC* right) {
  HASH_NODE* leftRes = left ? left->res : NULL;
  HASH_NODE* rightRes = right ? right->res : NULL;
  return tacJoin(
    tacJoin(left, right),
    tacCreate(TAC_ASS, leftRes, rightRes, 0)
  );
}

TAC* makeVec(TAC* result0, TAC* result1, TAC* result2) {
  return tacJoin(
    tacCreate(TAC_VEC, result0->res, result1->res, 0),
    result2);
}

TAC* makeVecAssignment(TAC* result0, TAC* result1, TAC* result2) {
  HASH_NODE* res0 = result0 ? result0->res : 0;
  HASH_NODE* res1 = result1 ? result1->res : 0;
  HASH_NODE* res2 = result2 ? result2->res : 0;
  return
  tacJoin(
    tacJoin(result0, result1),
    tacCreate(TAC_VEC_ASS, res0, res1, res2)
  );
}

TAC* makeIf(TAC* result0, TAC* result1, TAC* result2) {
  HASH_NODE* ifLabel = makeLabel();
  HASH_NODE* elseLabel = makeLabel();

  TAC* skipTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
  TAC* elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);

  TAC* ifTac = tacCreate(TAC_IFZ, ifLabel, result0 ? result0->res : 0, 0);
  TAC* ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);

  if (result2) {
    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(
      result0,
      ifTac),
      result1),
      skipTac),
      ifLabelTac),
      result2),
      elseLabelTac);
  } else {
    return tacJoin(tacJoin(tacJoin(
      result0,
      ifTac),
      result1),
      ifLabelTac);
  }
}

TAC* makeFunction(TAC* symbol, TAC* params, TAC* code) {
	return tacJoin(tacJoin(tacJoin(
    tacCreate(TAC_BEGINFUN, symbol->res, 0, 0),
    params),
    code),
    tacCreate(TAC_ENDFUN, symbol->res, 0, 0));
}

TAC* makeRead(TAC* target) {
  HASH_NODE* tempVar = makeTemp();
  return tacJoin(
    tacCreate(TAC_READ, tempVar, 0, 0),
    tacCreate(TAC_ASS, target->res, tempVar, 0)
  );
}

TAC* makePrint(TAC* result0, TAC* result1) {
  HASH_NODE* res0 = result0 ? result0->res : 0;
  return
    tacJoin(
      tacJoin(
        result0,
        tacCreate(TAC_PRINT, res0, 0, 0)),
        result1);
}

TAC* tacGenerate(AST* node, HASH_NODE* loopLabel) {
  TAC* result[MAX_NODES];

  if(!node) return 0;

  if(node->type == AST_LOOP) {
    loopLabel = makeLabel();
  }

  int i;
  for(i = 0; i < MAX_NODES; i++)
    result[i] = tacGenerate(node->son[i], loopLabel);

  switch(node->type)
  {
    //Symbols
    case AST_LIT_CHAR:
    case AST_LIT_FLOAT:
    case AST_LIT_INT:
    case AST_LIT_STRING:
    case AST_IDENTIFIER:
      return tacCreate(TAC_SYMBOL, node->symbol, 0, 0);

    //Operations
    case AST_EXP_SUM: return makeBinOp(TAC_ADD, result[0], result[1]);
    case AST_EXP_SUB: return makeBinOp(TAC_SUB, result[0], result[1]);
    case AST_EXP_MUL: return makeBinOp(TAC_MUL, result[0], result[1]);
    case AST_EXP_DIV: return makeBinOp(TAC_DIV, result[0], result[1]);
    case AST_EXP_LESS: return makeBinOp(TAC_LT, result[0], result[1]);
    case AST_EXP_GREATER: return makeBinOp(TAC_GT, result[0], result[1]);
    case AST_EXP_LE: return makeBinOp(TAC_LE, result[0], result[1]);
    case AST_EXP_GE: return makeBinOp(TAC_GE, result[0], result[1]);
    case AST_EXP_EQ: return makeBinOp(TAC_EQ, result[0], result[1]);
    case AST_EXP_DIF: return makeBinOp(TAC_DIF, result[0], result[1]);
    case AST_EXP_AND: return makeBinOp(TAC_AND, result[0], result[1]);
    case AST_EXP_OR: return makeBinOp(TAC_OR, result[0], result[1]);
    case AST_EXP_NOT: return makeBinOp(TAC_NOT, result[0], NULL);

    //Function declaration
    case AST_FUNC: return makeFunction(result[1], result[2], result[3]);
    case AST_FUNC_PARAMS: return result[0] ? result[0] : 0;
    case AST_PARAM_LIST: return tacJoin(tacCreate(TAC_PARAM, result[0]->res, 0, 0), result[1]);
    case AST_PARAM: return result[1];
    case AST_RETURN: return tacJoin(result[0], tacCreate(TAC_RETURN, result[0] ? result[0]->res : 0, 0, 0));

    //Function call
    // case AST_FUNC_CALL: return;
    // case AST_FUNC_ARGS: return;
    // case AST_ARG_LIST: return;

    //Var
    case AST_VARIABLE: return makeVar(result[1], result[2]);
    case AST_ASSIGN: return makeVar(result[0], result[1]);

    //vec
    case AST_VECTOR: return makeVec(result[1], result[2], result[3]);
    case AST_VEC_ASSIGN: return makeVecAssignment(result[0], result[1], result[2]);
    case AST_VECTOR_INIT: return result[0];
    case AST_LIT_LIST: return tacJoin(tacCreate(TAC_VEC_INIT, 0, result[0]->res, 0), result[1]);
    case AST_VECTOR_ACCS: return tacJoin(result[1], tacCreate(TAC_VEC_ACC, makeTemp(), result[0]->res, result[1]->res));

    //loop
    case AST_LOOP: return makeLoop(result[0], result[1], loopLabel);
		case AST_LEAP: return makeLeap(loopLabel);

    //if then else
    case AST_IF: return makeIf(result[0], result[1], result[2]);

    //read
    case AST_INPUT: return makeRead(result[0]);

    //print
    case AST_PRINT: return result[0];
    case AST_PRINT_LIST: return makePrint(result[0], result[1]);
    case AST_PRINT_EXP: return result[0];

    default: return tacJoin(tacJoin(tacJoin(result[0], result[1]), result[2]), result[3]);
  }

  return 0;
}
