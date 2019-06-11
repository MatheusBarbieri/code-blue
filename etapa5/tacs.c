#include "tacs.h"

TAC* makeBinOp(int type, TAC* result0, TAC* result1);
TAC* makeMove(int type, TAC* result0, TAC* result1);
TAC* makeMoveVec(int type, TAC* result0, TAC* result1, TAC* result2);
TAC* makeIfThen(TAC* result0, TAC* result1);
TAC* makeWhile(TAC* result0, TAC* result1);
TAC* makeFuncall(AST* node, TAC* result0, TAC* result1);
TAC* makePrint(AST* node,TAC* result0, TAC* result1);
TAC* makeFundec(AST* node, TAC* result0, TAC* result1, TAC* result2);
TAC* makeIfThenElse(TAC* result0, TAC* result1, TAC* result2);

TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2)
{
    TAC* newtac;

    newtac = (TAC*) calloc(1, sizeof(TAC));
    newtac->type = type;
    newtac->res = res;
    newtac->op1 = op1;
    newtac->op2 = op2;
    newtac->prev = 0;
    newtac->next = 0;

    return newtac;
}
void tacPrintSingle(TAC* tac)
{
    //fprintf(stderr, "ERRO - tacPrintSingle(TAC *tac);\n");
    if(!tac) return;

    if(tac->type == TAC_SYMBOL) return;

    fprintf(stderr, "TAC (");
    switch(tac->type)
    {
        case TAC_ADD: fprintf(stderr, "TAC_ADD");
            break;

        case TAC_SUB: fprintf(stderr, "TAC_SUB");
            break;

        case TAC_DIV: fprintf(stderr, "TAC_DIV");
            break;

        case TAC_MUL: fprintf(stderr, "TAC_MUL");
            break;

        case TAC_LT: fprintf(stderr, "TAC_LT");
            break;

        case TAC_GT: fprintf(stderr, "TAC_GT");
            break;

        case TAC_LE: fprintf(stderr, "TAC_LE");
            break;

        case TAC_GE: fprintf(stderr, "TAC_GE");
            break;

        case TAC_EQ: fprintf(stderr, "TAC_EQ");
            break;

        case TAC_OR: fprintf(stderr, "TAC_OR");
            break;

        case TAC_AND: fprintf(stderr, "TAC_AND");
            break;

        case TAC_BEGINFUN: fprintf(stderr, "TAC_BEGINFUN");
            break;

        case TAC_ENDFUN: fprintf(stderr, "TAC_ENDFUN");
            break;

        case TAC_RETURN: fprintf(stderr, "TAC_RET");
            break;

        case TAC_IFZ: fprintf(stderr, "TAC_IFZ");
            break;

        case TAC_JUMP: fprintf(stderr, "TAC_JUMP");
            break;

        case TAC_CALL: fprintf(stderr, "TAC_CALL");
            break;

        case TAC_ARG: fprintf(stderr, "TAC_ARG");
            break;

        case TAC_PRINT: fprintf(stderr, "TAC_PRINT");
            break;

        case TAC_READ: fprintf(stderr, "TAC_READ");
            break;

        case TAC_LABEL: fprintf(stderr, "TAC_LABEL");
            break;

        case TAC_MOVE: fprintf(stderr, "TAC_MOVE");
            break;

        case TAC_WHILE: fprintf(stderr, "TAC_WHILE");
            break;

        case TAC_VEC_WR: fprintf(stderr, "TAC_VEC_WR");
            break;

        case TAC_VEC_RD: fprintf(stderr, "TAC_VEC_RD");
            break;

        case TAC_FUNCALL: fprintf(stderr, "TAC_FUNCALL");
            break;

        case TAC_BUFFER: fprintf(stderr, "TAC_BUFFER");
            break;

        case TAC_ARG_LIST: fprintf(stderr, "TAC_ARG_LIST");
            break;

        case TAC_PARAM_LIST: fprintf(stderr, "TAC_PARAM_LIST");
            break;



        default: fprintf(stderr, "TAC_UNKNOWN");
            break;
    }

    if(tac->res) fprintf(stderr, ", %s", tac->res->text);
        else fprintf(stderr, ", 0");

    if(tac->op1) fprintf(stderr, ", %s", tac->op1->text);
        else fprintf(stderr, ", 0");

    if(tac->op2) fprintf(stderr, ", %s", tac->op2->text);
        else fprintf(stderr, ", 0");

    fprintf(stderr, ")\n");
}
void tacPrintBackwards(TAC* tac)
{
    if(!tac) return;

    tacPrintSingle(tac);
    tacPrintBackwards(tac->prev);
}

// CODE GENERATION

TAC* tacGenerate(AST* node)
{
    int i;
    TAC* result[MAX_NODES];

    if(!node || node->type == AST_PARAM_LIST) return 0;

    for(i = 0; i < MAX_NODES; i++)
        result[i] = tacGenerate(node->son[i]);

    switch(node->type)
    {
        case AST_INT:
        case AST_FLOAT:
        case AST_LIT_INT:
            return tacCreate(TAC_SYMBOL, node->symbol, 0,0);
            break;

        case AST_EXP_SUM:
            return makeBinOp(TAC_ADD, result[0], result[1]);
            break;

        case AST_EXP_SUB:
            return makeBinOp(TAC_SUB, result[0], result[1]);
            break;

        case AST_EXP_MUL:
            return makeBinOp(TAC_MUL, result[0], result[1]);
            break;

        case AST_EXP_DIV:
            return makeBinOp(TAC_DIV, result[0], result[1]);
            break;

        // case AST_LT:
        //     return makeBinOp(TAC_LT, result[0], result[1]);
        //     break;
        //
        // case AST_GT:
        //     return makeBinOp(TAC_GT, result[0], result[1]);
        //     break;
        //
        // case AST_LE:
        //     return makeBinOp(TAC_LE, result[0], result[1]);
        //     break;
        //
        // case AST_GE:
        //     return makeBinOp(TAC_GE, result[0], result[1]);
        //     break;
        //
        // case AST_EQ:
        //     return makeBinOp(TAC_EQ, result[0], result[1]);
        //     break;
        //
        // case AST_OR:
        //     return makeBinOp(TAC_OR, result[0], result[1]);
        //     break;

        case AST_RETURN:
            return makeBinOp(TAC_RETURN, result[0], result[1]);
            break;

        // case AST_READ:
        //     return tacCreate(TAC_READ, result[0]->res, 0, 0);
        //     break;
        //
        // case AST_AND:
        //     return makeBinOp(TAC_AND, result[0], result[1]);
        //     break;

        case AST_VARIABLE:
            return makeMove(TAC_MOVE, result[0], result[1]);
            break;

        case AST_VECTOR_ACCS:
            return makeMoveVec(TAC_VEC_WR, result[0], result[1], result[2]);
            break;

        case AST_VECTOR:
            return tacCreate(TAC_VEC_RD, makeTemp(), result[0]->res, result[1]->res);
            break;

        case AST_IF:
            return makeIfThen(result[0], result[1]);
            break;

        case AST_LOOP:
            return makeWhile(result[0], result[1]);
            break;

        case AST_FUNC_CALL:
            return makeFuncall(node, result[0], result[1]);
            break;

        case AST_PRINT:
            return makePrint(node, result[0], result[1]);
            break;

        //case AST_PARAM_LST:
        //    return tacJoin(tacCreate(TAC_PARAM_LIST, result[0]?result[0]->res:0, 0, 0),result[1]);
        //    break;

        //case AST_ARG_LIST:
        //    return tacJoin(tacCreate(TAC_ARG_LIST, result[0]?result[0]->res:0, 0, 0),result[1]);
        //    break;

        case AST_FUNC:
            return makeFundec(node, result[0], result[1], result[2]);
            break;

        default:
            return tacJoin(tacJoin(tacJoin(result[0], result[1]), result[2]), result[3]);
            break;
    }

    return 0;
}
TAC* tacJoin(TAC* l1, TAC* l2)
{
    if (l1 != NULL) {
      printf("operador 1 nao é vazio\n");
      if (l1->op1 != NULL) {
          printf("*** %d ***\n\n", l1->op1->type);
      } else {
        printf("operando é vazio\n");
      }
    } else {
      printf("operador 1 É VAZIO\n");
    }

    // if (l1 != NULL) {
    //   printf("%s", l2->op1->text);
    //   printf("%s", l2->op2->text);
    //   printf("operador 2 nao é vazio\n");
    // } else {
    //   printf("operador 2 É VAZIO\n");
    // }





    TAC* aux;

    if(!l1) return l2;

    if(!l2) return l1;

    for(aux = l2; aux->prev; aux = aux->prev);

    aux->prev = l1;

    return l2;
}

TAC* tacReverse(TAC* tac)
{
    if(!tac) return 0;

    for(; tac->prev; tac = tac->prev)
        tac->prev->next = tac;

    return tac;
}

void tacPrintForward(TAC* tac)
{
    //fprintf(stderr, "ERRO - tacPrintForward\n");
    if(!tac) return;

    tacPrintSingle(tac);
    tacPrintForward(tac->next);
}

TAC* makeBinOp(int type, TAC* result0, TAC* result1)
{
    printf("fazendo operacao binaria.\n");
    return tacJoin(tacJoin(result0, result1), tacCreate(type, makeTemp(), result0?result0->res:0, result1?result1->res:0));
}

TAC* makeMove(int type, TAC* result0, TAC* result1)
{
    return tacJoin(tacJoin(result0, result1), tacCreate(type, result0?result0->res:0, 0, result1?result1->res:0));
}

TAC* makeMoveVec(int type, TAC* result0, TAC* result1, TAC* result2)
{
    return tacJoin(tacJoin(result0, result1), tacCreate(type, result0?result0->res:0, result1?result1->res:0, result2?result2->res:0));
}

TAC* makeIfThen(TAC* result0, TAC* result1)
{
    TAC* newIfTac = 0;
    TAC* newLabelTac = 0;
    HASH_NODE* newLabel = 0;

    newLabel = makeLabel();
    newIfTac = tacCreate(TAC_IFZ, newLabel, result0?result0->res:0, 0);
    newLabelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(result0, newIfTac), result1), newLabelTac);
}

TAC* makeIfThenElse(TAC* result0, TAC* result1, TAC* result2)
{
    TAC* skipTac = 0;
    TAC* elseLabelTac = 0;

    TAC* ifTac = 0;
    TAC* ifLabelTac = 0;

    HASH_NODE* ifLabel = makeLabel();
    HASH_NODE* elseLabel = makeLabel();

    skipTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
    elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);

    ifTac = tacCreate(TAC_IFZ, ifLabel, result0?result0->res:0, 0);
    ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(result0, ifTac), result1), skipTac), ifLabelTac), result2), elseLabelTac);

}

TAC* makeWhile(TAC* result0, TAC* result1)
{
    TAC* whileTac = 0;
    TAC* labelWhileTac = 0;

    TAC* jumpLabelTac = 0;
    TAC* jumpTac = 0;

    HASH_NODE* whileLabel = 0;
    HASH_NODE* jumpLabel = 0;

    whileLabel = makeLabel();
    jumpLabel = makeLabel();

    jumpTac = tacCreate(TAC_JUMP, whileLabel, 0, 0);
    whileTac = tacCreate(TAC_IFZ, jumpLabel, result0?result0->res:0, 0);

    labelWhileTac = tacCreate(TAC_LABEL, whileLabel, 0, 0);
    jumpLabelTac = tacCreate(TAC_LABEL, jumpLabel, 0, 0);

    return tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(labelWhileTac, result0), whileTac), result1), jumpTac), jumpLabelTac);
}

TAC* makeFuncall(AST* node, TAC* result0, TAC* result1)
{
    int count = 0;
    char countStr[2];

    TAC* resultParam;
    TAC* bufferTac = 0;
    TAC* funcallTac = 0;
    TAC* firstJoin = 0;
    TAC* paramTac = 0;


    bufferTac = tacCreate(TAC_BUFFER, result0?result0->res:0, 0, 0);
    bufferTac->res = makeTemp();

    funcallTac = tacCreate(TAC_FUNCALL, bufferTac?bufferTac->res:0, result0?result0->res:0, 0);

    firstJoin = tacJoin(tacJoin(result0, funcallTac), result1);

    while(node->son[1])
    {
        count++;
        sprintf(countStr, "%d", count);
        //TODO: escalar??
        //HASH_NODE* countNode = hashInsert(SYMBOL_SCALAR, countStr);
        HASH_NODE* countNode = hashInsert(AST_LIT_INT, countStr);

        node = node->son[1];

        resultParam = tacGenerate(node->son[0]);
        paramTac = tacCreate(TAC_ARG_LIST, resultParam?resultParam->res:0, result0?result0->res:0, countNode);
        tacJoin(paramTac, firstJoin);
    }

    return firstJoin;
}

TAC* makePrint(AST* node, TAC* result0, TAC* result1)
{
    printf("aceitou print\n");
    AST * symbolNode = 0;

    int count = 1;
    char countStr[2];

    TAC* printTac = 0;
    TAC* secondTac = 0;
    TAC* expNode = 0;

    node = node->son[0];
    symbolNode = node->son[0];

    if(symbolNode->type == AST_EXP_SUM || symbolNode->type == AST_EXP_SUB
    || symbolNode->type == AST_EXP_MUL || symbolNode->type == AST_EXP_DIV)
    //TODO: INCLUIR???
    // || symbolNode->type == AST_LT || symbolNode->type == AST_GT
    // || symbolNode->type == AST_LE || symbolNode->type == AST_NOT
    // || symbolNode->type == AST_EQ || symbolNode->type == AST_OR
    // || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
        expNode = tacGenerate(symbolNode);

    //fprintf(stderr, "param: %s; num %d \n", symbolNode->symbol->text, count);

    sprintf(countStr, "%d", count);
    //TODO: AST_LIT_INT ou ESCALAR???
    //HASH_NODE* countNode = hashInsert(SYMBOL_SCALAR, countStr);
    HASH_NODE* countNode = hashInsert(AST_LIT_INT, countStr);

    //TODO????
    //printTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_DECL || node->son[0]->type == AST_LIT_STRING)  ? node->son[0]->symbol : expNode->res, 0, countNode);
    printTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_DECL || node->son[0]->type == AST_LIT_STRING)  ? node->son[0]->symbol : expNode->res, 0, countNode);

    if(symbolNode->type == AST_EXP_SUM || symbolNode->type == AST_EXP_SUB || symbolNode->type == AST_EXP_MUL || symbolNode->type == AST_EXP_DIV)// || symbolNode->type == AST_LT || symbolNode->type == AST_GT || symbolNode->type == AST_LE || symbolNode->type == AST_NOT || symbolNode->type == AST_EQ || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
    {
        expNode = tacJoin(result0, expNode);
        printTac = tacJoin(expNode, printTac);
    }
    else
    {
        printTac = tacJoin(result0, printTac);
    }


    while(node->son[1])
    {
        count++;
        sprintf(countStr, "%d", count);

        // TODO: SCALAR???
        //HASH_NODE* countNode = hashInsert(SYMBOL_SCALAR, countStr);
        HASH_NODE* countNode = hashInsert(AST_LIT_INT, countStr);

        node = node->son[1];
        symbolNode = node->son[0];

        if(symbolNode->type == AST_VECTOR)
            symbolNode = symbolNode->son[0];
        else if(symbolNode->type == AST_EXP_SUM || symbolNode->type == AST_EXP_SUB || symbolNode->type == AST_EXP_MUL || symbolNode->type == AST_EXP_DIV) //|| symbolNode->type == AST_LT || symbolNode->type == AST_GT || symbolNode->type == AST_LE || symbolNode->type == AST_NOT || symbolNode->type == AST_EQ || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC)
            expNode = tacGenerate(symbolNode);

        //TODO: AST_SYMBOL ?? AST_STRING??
        //secondTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_SYMBOL || node->son[0]->type == AST_STRING) ? node->son[0]->symbol : expNode->res, 0, countNode);
        secondTac = tacCreate(TAC_PRINT, (node->son[0]->type == AST_DECL || node->son[0]->type == AST_LIT_STRING) ? node->son[0]->symbol : expNode->res, 0, countNode);
        //fprintf(stderr, "param: %s; num %d \n", symbolNode->symbol->text, count);

        if(symbolNode->type == AST_EXP_SUM || symbolNode->type == AST_EXP_SUB || symbolNode->type == AST_EXP_MUL || symbolNode->type == AST_EXP_DIV) {// || symbolNode->type == AST_LT || symbolNode->type == AST_GT || symbolNode->type == AST_LE || symbolNode->type == AST_NOT || symbolNode->type == AST_EQ || symbolNode->type == AST_OR || symbolNode->type == AST_AND || symbolNode->type == AST_VEC){
            expNode = tacJoin(expNode, secondTac);
            secondTac = tacJoin(expNode, printTac);
        }
        else
        {
            secondTac = tacJoin(secondTac, printTac);
        }

    }

    return tacJoin(secondTac, result1);
}

TAC* makeFundec(AST* node,TAC* result0, TAC* result1, TAC* result2)
{
    int count = 0;
    char countStr[2];

    TAC* beginTac = 0;
    TAC* endTac = 0;
    TAC* firstJoin = 0;
    TAC* paramTac = 0;
    TAC* resultParam = 0;

    beginTac = tacCreate(TAC_BEGINFUN, result0?result0->res:0, 0,  0);
    endTac = tacCreate(TAC_ENDFUN, result0?result0->res:0, 0, 0);

    firstJoin = tacJoin(tacJoin(tacJoin(beginTac, result1), result2 ), endTac);

    while(node->son[1])
    {
        count++;
        node = node->son[1];

        sprintf(countStr, "%d", count);
        //TODO: ESCALAR??
        //HASH_NODE* countNode = hashInsert(SYMBOL_SCALAR, countStr);
        HASH_NODE* countNode = hashInsert(AST_LIT_INT, countStr);

        resultParam = tacGenerate(node->son[0]);
        paramTac = tacCreate(TAC_PARAM_LIST, resultParam?resultParam->res:0, result0?result0->res:0, countNode);
        tacJoin(paramTac, firstJoin);
    }

    return firstJoin;
}
