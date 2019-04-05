%{
#include <stdio.h>
#include <stdlib.h>

int yylex(void);
void yyerror (char const *s);
extern int getLineNumber();
%}

%token KW_BYTE
%token KW_INT
%token KW_FLOAT

%token KW_IF
%precedence KW_THEN
%precedence KW_ELSE

%token KW_LOOP
%token KW_LEAP

%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token LIT_INTEGER
%token LIT_FLOAT
%token LIT_CHAR
%token LIT_STRING

%token TK_IDENTIFIER
%token TOKEN_ERROR

%start entry

%left '+' '-'
%left '*' '/' '%'
%left '^'
%left OPERATOR_OR
%left OPERATOR_AND
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF

%right OPERATOR_NOT

%%

entry: program | %empty ;

program: declaration program | declaration ;

  //declarations
declaration: variable
           | vector
           | function
           ;

  //variable
variable: primitive_type TK_IDENTIFIER '=' literal ';'

  //vector
vector: primitive_type TK_IDENTIFIER '[' LIT_INTEGER ']' vector_initialization ';'

vector_initialization: ':' literal_list
                     | %empty
                     ;

vector_acess: TK_IDENTIFIER '[' expression ']'

  // Tipos Primitivos
primitive_type: KW_BYTE
              | KW_INT
              | KW_FLOAT
              ;

  // Literals
literal: LIT_INTEGER
       | LIT_FLOAT
       | LIT_CHAR
       ;

literal_list: literal literal_list
            | literal
            ;

 // function
function: primitive_type TK_IDENTIFIER function_parameters command_block ';' ;

function_parameters: '(' param_list ')'
                   | '(' ')'
                   ;

  // function_call
function_call: TK_IDENTIFIER arguments ;

arguments: '(' ')' | '(' argument_list ')' ;

argument_list: expression | argument_list ',' expression ;

  // functions parameter list
param_list: param ',' param_list
          | param
          ;

param: primitive_type TK_IDENTIFIER ;

 // command_block
command_block: '{' command_list '}' ;

command_list: command
            | command ';' command_list
            ;

  // command
command: attribution
       | flow_control
       | input_statement
       | output_statement
       | return_statement
       | command_block
       | %empty
       ;

  // attribution
attribution: TK_IDENTIFIER '=' expression
           | vector_acess '=' expression
           ;

  //flow control
flow_control: if_statement
            | loop_statement
            | KW_LEAP
            ;

  // input statement
input_statement: KW_READ TK_IDENTIFIER ;

  // output statement
output_statement: KW_PRINT printables ;

  // List of literals that includes string for *printing* porpouses
printables: printable ',' printables
          | printable
          ;

printable: LIT_STRING | expression ;

  // return statement
return_statement: KW_RETURN expression ;

  // if statement
if_statement: KW_IF '(' expression ')' KW_THEN command ;
            | KW_IF '(' expression ')' KW_THEN command KW_ELSE command ;

  // loop
loop_statement: KW_LOOP '(' expression ')' command ;

  //expression
expression: '(' expression ')'
          | literal
          | function_call
          | TK_IDENTIFIER
          | vector_acess
          | expression '+' expression
          | expression '-' expression
          | expression '*' expression
          | expression '/' expression
          | expression '%' expression
          | expression '^' expression
          | expression '<' expression
          | expression '>' expression
          | expression OPERATOR_LE expression
          | expression OPERATOR_GE expression
          | expression OPERATOR_EQ expression
          | expression OPERATOR_DIF expression
          | expression OPERATOR_AND expression
          | expression OPERATOR_OR expression
          | OPERATOR_NOT expression
          ;

%%

void yyerror(const char *msg) {
    printf("Error on line %d: %s\n", getLineNumber(), msg);
    exit(3);
}
