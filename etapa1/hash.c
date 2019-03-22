#include <stdlib.h>
#include "hash.h"
#include "lex.yy.c"

int hashAddress(char *text) {
  int address = 1;
  int i;
  for (i = 0; i < HASH_SIZE; i++) {
    address = (address*text[i]) % HASH_SIZE + 1;
  }
  return address - 1;
}

struct hashNode* hashInsert(int type, char *text) {
  struct hashNode *newnode;
  newnode = (struct hashNode *)calloc(1, sizeof(struct hashNode));
  newnode->type = type;
  newnode->text = calloc (strlen(yytext)+1, sizeof(char));
  strcpy(newnode->text, text);
  int address = hashAddress(text);
  newnode->next = Table[address];
  Table[address] = newnode;
  return newnode;
}

void hashPrint(void) {
  //int i;*
//  NODE *node;

}

//NODE hashFind(char *text)
