#include <stdio.h>

#define HASH_SIZE 30

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
