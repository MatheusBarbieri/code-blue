%{
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"

int yylex(void);
void yyerror (char const *s);
extern int getLineNumber();
AST* root = NULL;
%}

%union {
	struct hashNode* symbol;
	struct ast_node* ast;
}

%token KW_BYTE
%token KW_INT
%token KW_FLOAT

%token KW_IF

%token KW_LOOP
%token KW_LEAP

%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token<symbol> LIT_INTEGER
%token<symbol> LIT_FLOAT
%token<symbol> LIT_CHAR
%token<symbol> LIT_STRING

%token TK_IDENTIFIER
%token TOKEN_ERROR

%type<ast> entry
%type<ast> program
%type<ast> declaration
%type<ast> variable
%type<ast> vector
%type<ast> vector_initialization
%type<ast> vector_access
%type<ast> primitive_type
%type<ast> literal
%type<ast> literal_list
%type<ast> function
%type<ast> function_parameters
%type<ast> function_call
%type<ast> arguments
%type<ast> argument_list
%type<ast> param_list
%type<ast> param
%type<ast> command_block
%type<ast> command_list
%type<ast> command
%type<ast> assignment
%type<ast> flow_control
%type<ast> input_statement
%type<ast> output_statement
%type<ast> printables
%type<ast> printable
%type<ast> return_statement
%type<ast> if_statement
%type<ast> loop_statement
%type<ast> expression

%start entry

%right KW_THEN KW_ELSE

%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left OPERATOR_OR
%left OPERATOR_AND
%left '+' '-'
%left '*' '/' '%'
%left '^'

%right OPERATOR_NOT

%%

entry: program { astPrint(0, root); }
		 | { astPrint(0, root); }
     ;

program: declaration program { root = $$ = astCreate(AST_DEC, 0, $1, $2, 0, 0); }
			 | declaration { root = $$ = astCreate(AST_DEC, 0, $1, 0, 0, 0); } ;

  //declarations
declaration: variable { $$ = $1 } ;
           | vector { $$ = astCreate(AST_VET, 0, $1, 0, 0, 0); } ;
           | function { $$ = astCreate(AST_FUN, 0, $1, 0, 0, 0); } ;
           ;

  //variable
variable: primitive_type TK_IDENTIFIER '=' literal ';' { astCreate(AST_VAR, 0, $1, $4, 0, 0); }

  //vector
vector: primitive_type TK_IDENTIFIER '[' LIT_INTEGER ']' vector_initialization ';'

vector_initialization: ':' literal_list
                     |
                     ;

vector_access: TK_IDENTIFIER '[' expression ']'

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
command: assignment
       | flow_control
       | input_statement
       | output_statement
       | return_statement
       | command_block
       |
       ;

  // assignment
assignment: TK_IDENTIFIER '=' expression
          | vector_access '=' expression
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

printable: LIT_STRING
         | expression
         ;

  // return statement
return_statement: KW_RETURN expression ;

  // if statement
if_statement: KW_IF '(' expression ')' KW_THEN command
            | KW_IF '(' expression ')' KW_THEN command KW_ELSE command
            ;

  // loop
loop_statement: KW_LOOP '(' expression ')' command ;

  //expression
expression: '(' expression ')'
          | literal
          | function_call
          | TK_IDENTIFIER
          | vector_access
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
