#include <stdlib.h>
#include <string.h>
#include "hash.h"

int hashAddress(char* text) {
  int address = 1;
  int i;
  for (i = 0; i < strlen(text); i++) {
    address = (address*text[i]) % HASH_SIZE + 1;
  }
  return address - 1;
}

struct hashNode* hashInsert(int type, char* text) {
  struct hashNode *foundNode = hashFind(text);
  if (foundNode) return foundNode;

  struct hashNode *newNode = (struct hashNode *)calloc(1, sizeof(struct hashNode));

  newNode->type = type;
  newNode->params = NULL;
  newNode->text = calloc(strlen(text)+1, sizeof(char));
  strcpy(newNode->text, text);
  int address = hashAddress(text);
  newNode->next = Table[address];
  Table[address] = newNode;
  return newNode;
}

struct hashNode* hashFind(char* text) {
  struct hashNode *node;
  int address = hashAddress(text);
  for (node = Table[address]; node; node = node->next) {
    if (strcmp(text, node->text) == 0)
      return node;
  }
  return 0;
}

void hashPrint(void) {
  int i;
  struct hashNode *node;
  for (i = 0; i < HASH_SIZE; i++) {
    for (node = Table[i]; node; node = node->next) {
      fprintf(stderr, "Table[%d] has %s\n", i, node->text);
    }
  }
}
