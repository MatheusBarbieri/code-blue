//Includes
#include <stdio.h>
#include "tokens.h"
#include "hash.c"
//Defines
#define print_nome(TOKEN) \
  printf("%d " #TOKEN " [%s]\n", getLineNumber(), yytext);
#define print_nome2(TOKEN) \
  printf("%d TK_ESPECIAL [%c]\n", getLineNumber(), TOKEN);
//
// #define print_nome(TOKEN) \
//   printf(#TOKEN);
// #define print_nome2(TOKEN) \
//   printf("%c", TOKEN);

//Externs
extern char *yytext;
extern FILE *yyin;

extern int yylex();
extern int getLineNumber();
extern int isRunning();

//Main
int main (int argc, char **argv)
{
  int token = 0;
  while ((isRunning())) {
    token = yylex();
    switch (token){
    case ',':
    case ';':
    case ':':
    case '(':
    case ')':
    case '[':
    case ']':
    case '{':
    case '}':
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '>':
    case '=':
    case '!':
    case '&':
    case '#':
    case '$': print_nome2 (token); break;
    case KW_BYTE: print_nome(KW_BYTE); break;
    case KW_INT: print_nome(KW_INT); break;
    case KW_FLOAT: print_nome(KW_FLOAT); break;
    case KW_IF: print_nome(KW_IF); break;
    case KW_THEN: print_nome(KW_THEN); break;
    case KW_ELSE: print_nome(KW_ELSE); break;
    case KW_LOOP: print_nome(KW_LOOP); break;
    case KW_LEAP: print_nome(KW_LEAP); break;
    case KW_READ: print_nome(KW_READ); break;
    case KW_RETURN: print_nome(KW_RETURN); break;
    case KW_PRINT: print_nome(KW_PRINT); break;
    case OPERATOR_LE: print_nome(OPERATOR_LE); break;
    case OPERATOR_GE: print_nome(OPERATOR_GE); break;
    case OPERATOR_EQ: print_nome(OPERATOR_EQ); break;
    case OPERATOR_DIF: print_nome(OPERATOR_DIF); break;
    case OPERATOR_OR: print_nome(OPERATOR_OR); break;
    case OPERATOR_AND: print_nome(OPERATOR_AND); break;
    case OPERATOR_NOT: print_nome(OPERATOR_NOT); break;
    case LIT_INTEGER: print_nome(LIT_INTEGER); break;
    case LIT_FLOAT: print_nome(LIT_FLOAT); break;
    case LIT_CHAR: print_nome(LIT_CHAR); break;
    case LIT_STRING: print_nome(LIT_STRING); break;
    case TK_IDENTIFIER: print_nome(TK_IDENTIFIER); break;
    case TOKEN_ERROR:  print_nome(TOKEN_ERROR); break;
    case 0: return 0;
    default: printf ("<Não deve chegar aqui: code %d>\n", token); return 1;
    }
  }
  return 0;
}
