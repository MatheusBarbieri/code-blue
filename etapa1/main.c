//Includes
#include <stdio.h>
#include "tokens.h"

//Defines
#define print_token(TOKEN) printf("%d " #TOKEN " [%s]\n", getLineNumber(), yytext);
#define print_special(TOKEN) printf("%d TK_ESPECIAL [%c]\n", getLineNumber(), TOKEN);

//Externs
extern char *yytext;
extern FILE *yyin;

int yylex();
int getLineNumber();
int isRunning();
void hashPrint();

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
    case '$': print_special (token); break;
    case KW_BYTE: print_token(KW_BYTE); break;
    case KW_INT: print_token(KW_INT); break;
    case KW_FLOAT: print_token(KW_FLOAT); break;
    case KW_IF: print_token(KW_IF); break;
    case KW_THEN: print_token(KW_THEN); break;
    case KW_ELSE: print_token(KW_ELSE); break;
    case KW_LOOP: print_token(KW_LOOP); break;
    case KW_LEAP: print_token(KW_LEAP); break;
    case KW_READ: print_token(KW_READ); break;
    case KW_RETURN: print_token(KW_RETURN); break;
    case KW_PRINT: print_token(KW_PRINT); break;
    case OPERATOR_LE: print_token(OPERATOR_LE); break;
    case OPERATOR_GE: print_token(OPERATOR_GE); break;
    case OPERATOR_EQ: print_token(OPERATOR_EQ); break;
    case OPERATOR_DIF: print_token(OPERATOR_DIF); break;
    case OPERATOR_OR: print_token(OPERATOR_OR); break;
    case OPERATOR_AND: print_token(OPERATOR_AND); break;
    case OPERATOR_NOT: print_token(OPERATOR_NOT); break;
    case LIT_INTEGER: print_token(LIT_INTEGER); break;
    case LIT_FLOAT: print_token(LIT_FLOAT); break;
    case LIT_CHAR: print_token(LIT_CHAR); break;
    case LIT_STRING: print_token(LIT_STRING); break;
    case TK_IDENTIFIER: print_token(TK_IDENTIFIER); break;
    case TOKEN_ERROR:  print_token(TOKEN_ERROR); break;
    case 0: printf ("\n<Fim da execução>\n\n"); hashPrint(); return 0;
    default: printf ("<Não deve chegar aqui: %d>\n", token); return 1;
    }
  }
}
