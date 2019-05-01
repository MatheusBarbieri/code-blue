%{
#include "parser.tab.h"
#include "hash.c"
int lineCount = 1;
int running = 1;
int getLineNumber();
int isRunning();
%}

%x COMMENT_BLOCK

%%

[ \t]+ { }

\r?\n { lineCount++; }

"ZZ".* { }
"XXX".* BEGIN(COMMENT_BLOCK);
<COMMENT_BLOCK>.*"XXX" BEGIN(INITIAL);
<COMMENT_BLOCK>\n { lineCount++; }
<COMMENT_BLOCK>. { }


[,;:()\[\]{}+\-*/<>=!&$#] {
    return yytext[0];
}

"byte" { return KW_BYTE; }
"int" { return KW_INT; }
"float" { return KW_FLOAT; }
"if" { return KW_IF; }
"then" { return KW_THEN; }
"else" { return KW_ELSE; }
"loop" { return KW_LOOP; }
"leap" { return KW_LEAP; }
"read" { return KW_READ; }
"return" { return KW_RETURN; }
"print" { return KW_PRINT; }
"<=" { return OPERATOR_LE; }
">=" { return OPERATOR_GE; }
"==" { return OPERATOR_EQ; }
"!=" { return OPERATOR_DIF; }
"or" { return OPERATOR_OR; }
"and" { return OPERATOR_AND; }
"not" { return OPERATOR_NOT; }

[OIHGFEDCBA]+ { yylval.symbol = hashInsert(LIT_INTEGER, yytext); return LIT_INTEGER; }
[OIHGFEDCBA]+"."[OIHGFEDCBA]+ { yylval.symbol = hashInsert(LIT_FLOAT, yytext); return LIT_FLOAT; }
[´'].[´'] { yylval.symbol = hashInsert(LIT_CHAR, yytext); return LIT_CHAR; }
\"([^\\"]|\\.)*\" { yylval.symbol = hashInsert(LIT_STRING, yytext); return LIT_STRING; }

[a-z_0-9]* { yylval.symbol = hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER; }

. {return TOKEN_ERROR; }

%%

int getLineNumber() { return lineCount; }
int isRunning() { return running; }
void initMe() { }

int yywrap() {
  running = 0;
  return 1;
}
