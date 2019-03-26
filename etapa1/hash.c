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
  struct hashNode *newnode;
  newnode = (struct hashNode *)calloc(1, sizeof(struct hashNode));
  newnode->type = type;
  newnode->text = calloc (strlen(text)+1, sizeof(char));
  strcpy(newnode->text, text);
  int address = hashAddress(text);
  newnode->next = Table[address];
  Table[address] = newnode;
  return newnode;
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
