#ifndef HASH_HEADER
#define HASH_HEADER

#include <stdio.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INTEGER 100
#define SYMBOL_LIT_FLOAT 101
#define SYMBOL_LIT_CHAR 102
#define SYMBOL_TK_IDENTIFIER 103

#define SYMBOL_VAR 104
#define SYMBOL_VECTOR	105
#define SYMBOL_FUNCTION 106

#define DATATYPE_INTEGER 107
#define DATATYPE_FLOAT 108
#define DATATYPE_BOOL 109

struct paramList {
  struct hashNode *symbol;
  struct paramList *next;
};

struct hashNode {
  int type;
  int datatype;
  paramList *params;
  char *text;
  struct hashNode *next;
};

struct hashNode *Table[HASH_SIZE];

int hashAddress(char *text);
struct hashNode* hashInsert(int type, char *text);
struct hashNode* hashFind(char *text);
void hashPrint(void);

#endif
