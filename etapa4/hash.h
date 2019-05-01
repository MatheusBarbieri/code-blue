#include <stdio.h>

#define HASH_SIZE 997

#define SYMBOL_LITINT 1
#define SYMBOL_IDENTIFIER 2

#define SYMBOL_VAR 3
#define SYMBOL_FUN 4


struct hashNode {
  int type;
  char *text;
  struct hashNode *next;
};

struct hashNode *Table[HASH_SIZE];

int hashAddress(char *text);
struct hashNode* hashInsert(int type, char *text);
struct hashNode* hashFind(char *text);
void hashPrint(void);
