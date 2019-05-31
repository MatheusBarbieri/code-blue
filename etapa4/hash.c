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

HASH_NODE* hashInsert(int type, char* text) {
  HASH_NODE *foundNode = hashFind(text);
  if (foundNode) return foundNode;

  HASH_NODE *newNode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));

  newNode->type = type;
  newNode->params = NULL;
  newNode->text = calloc(strlen(text)+1, sizeof(char));
  strcpy(newNode->text, text);
  int address = hashAddress(text);
  newNode->next = Table[address];
  Table[address] = newNode;
  return newNode;
}

HASH_NODE* hashFind(char* text) {
  HASH_NODE *node;
  int address = hashAddress(text);
  for (node = Table[address]; node; node = node->next) {
    if (strcmp(text, node->text) == 0)
      return node;
  }
  return 0;
}

void hashPrint(void) {
  int i;
  HASH_NODE *node;
  for (i = 0; i < HASH_SIZE; i++) {
    for (node = Table[i]; node; node = node->next) {
      fprintf(stderr, "Table[%d] has %s\n", i, node->text);
    }
  }
}
