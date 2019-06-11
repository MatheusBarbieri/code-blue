%{
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "ast.h"
#include "semantic.h"
#include "tacs.h"

int yylex(void);
void yyerror (char const *s);
int getLineNumber();
AST* getAST();
AST* root = NULL;
%}

%union {
	struct hashNode* symbol;
	struct astNode* ast;
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

%token<symbol> TK_IDENTIFIER
%token TOKEN_ERROR

%type<ast> entry
%type<ast> program
%type<ast> declaration
%type<ast> variable
%type<ast> vector
%type<ast> vector_size
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
%type<ast> identifier

%start entry

%right KW_THEN KW_ELSE

%left OPERATOR_OR
%left OPERATOR_AND

%left OPERATOR_EQ OPERATOR_DIF
%left '<' '>' OPERATOR_LE OPERATOR_GE

%left '+' '-'
%left '*' '/' '%'
%left '^'

%right OPERATOR_NOT

%%

entry: program { root = $1; semanticCheck($1); }
	 ;

program: declaration program { $$ = astCreate(AST_DECL, 0, $1, $2, 0, 0); }
	   | declaration { $$ = astCreate(AST_DECL, 0, $1, 0, 0, 0); }
	   ;

  //declarations
declaration: variable { $$ = $1; }
           | vector { $$ = $1; }
           | function { $$ = $1; }
           ;

  //variable
variable: primitive_type identifier '=' literal ';' { $$ = astCreate(AST_VARIABLE, 0, $1, $2, $4, 0); }
		;

  //vector
vector: primitive_type identifier '[' vector_size ']' vector_initialization ';' { $$ = astCreate(AST_VECTOR, 0, $1, $2, $4, $6); }
	  ;

vector_size: LIT_INTEGER { $$ = astCreate(AST_LIT_INT, $1, 0, 0, 0, 0); }
					 | LIT_CHAR { $$ = astCreate(AST_LIT_CHAR, $1, 0, 0, 0, 0); }
					 | identifier
					 ;

vector_initialization: ':' literal_list { $$ = astCreate(AST_VECTOR_INIT, 0, $2, 0, 0, 0); }
                     | { $$ = NULL; }
                     ;

vector_access: identifier '[' expression ']' { $$ = astCreate(AST_VECTOR_ACCS, 0, $1, $3, 0, 0); }
			 ;

  // Tipos Primitivos
primitive_type: KW_BYTE  { $$ = astCreate(AST_BYTE, 0, 0, 0, 0, 0); }
              | KW_INT 	 { $$ = astCreate(AST_INT, 0, 0, 0, 0, 0); }
              | KW_FLOAT { $$ = astCreate(AST_FLOAT, 0, 0, 0, 0, 0); }
              ;

  // Literals
literal: LIT_INTEGER	{ $$ = astCreate(AST_LIT_INT, $1, 0, 0, 0, 0); }
       | LIT_FLOAT		{ $$ = astCreate(AST_LIT_FLOAT, $1, 0, 0, 0, 0); }
       | LIT_CHAR		{ $$ = astCreate(AST_LIT_CHAR, $1, 0, 0, 0, 0); }
       ;

literal_list: literal literal_list { $$ = astCreate(AST_LIT_LIST, 0, $1, $2, 0, 0); }
            | literal { $$ = astCreate(AST_LIT_LIST, 0, $1, 0, 0, 0); }
            ;

 // function
function: primitive_type identifier function_parameters command_block ';' { $$ = astCreate(AST_FUNC, 0, $1, $2, $3, $4); }
        ;

function_parameters: '(' param_list ')' { $$ = astCreate(AST_FUNC_PARAMS, 0, $2, 0, 0, 0); }
                   | '(' ')' { $$ = astCreate(AST_FUNC_PARAMS, 0, 0, 0, 0, 0); }
                   ;

  // function_call
function_call: identifier arguments { $$ = astCreate(AST_FUNC_CALL, 0, $1, $2, 0, 0); }
             ;

arguments: '(' argument_list ')' { $$ = astCreate(AST_FUNC_ARGS, 0, $2, 0, 0, 0); }
		 | '(' ')' { $$ = astCreate(AST_FUNC_ARGS, 0, 0, 0, 0, 0); }
		 ;

argument_list: expression ',' argument_list { $$ = astCreate(AST_ARG_LIST, 0, $1, $3, 0, 0); }
			 | expression { $$ = astCreate(AST_ARG_LIST, 0, $1, 0, 0, 0); }
			 ;

  // functions parameter list
param_list: param ',' param_list { $$ = astCreate(AST_PARAM_LIST, 0, $1, $3, 0, 0); }
          | param { $$ = astCreate(AST_PARAM_LIST, 0, $1, 0, 0, 0); }
          ;

param: primitive_type identifier { $$ = astCreate(AST_PARAM, 0, $1, $2, 0, 0); }

 // command_block
command_block: '{' command_list '}' { $$ = astCreate(AST_CMD_BLOCK, 0, $2, 0, 0, 0); }
             ;

command_list: command ';' command_list { $$ = astCreate(AST_CMD_LIST, 0, $1, $3, 0, 0); }
            | command { $$ = astCreate(AST_CMD_LIST, 0, $1, 0, 0, 0); }
            ;

  // command
command: assignment { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | flow_control { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | input_statement { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | output_statement { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | return_statement { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | command_block { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0); }
       | { $$ = astCreate(AST_CMD, 0, 0, 0, 0, 0); }
       ;

  // assignment
assignment: identifier '=' expression { $$ = astCreate(AST_ASSING, 0, $1, $3, 0, 0); }
          | vector_access '=' expression { $$ = astCreate(AST_ASSING, 0, $1, $3, 0, 0); }
          ;

  //flow control
flow_control: if_statement { $$ = $1; }
            | loop_statement { $$ = $1; }
            | KW_LEAP { $$ = astCreate(AST_LEAP, 0, 0, 0, 0, 0); }
            ;

  // input statement
input_statement: KW_READ identifier { $$ = astCreate(AST_INPUT, 0, $2, 0, 0, 0); }
			   ;

  // output statement
output_statement: KW_PRINT printables { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0); }
				;

  // List of literals that includes string for *printing* porpouses
printables: printable ',' printables { $$ = astCreate(AST_PRINT_LIST, 0, $1, $3, 0, 0); }
          | printable { $$ = astCreate(AST_PRINT_LIST, 0, $1, 0, 0, 0); }
          ;

printable: LIT_STRING { $$ = astCreate(AST_LIT_STRING, $1, 0, 0, 0, 0); }
         | expression { $$ = astCreate(AST_PRINT_EXP, 0, $1, 0, 0, 0); }
         ;

  // return statement
return_statement: KW_RETURN expression { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0); }
				;

  // if statement
if_statement: KW_IF '(' expression ')' KW_THEN command  { $$ = astCreate(AST_IF, 0, $3, $6, 0, 0); }
            | KW_IF '(' expression ')' KW_THEN command KW_ELSE command { $$ = astCreate(AST_IF, 0, $3, $6, $8, 0); }
            ;

  // loop
loop_statement: KW_LOOP '(' expression ')' command { $$ = astCreate(AST_LOOP, 0, $3, $5, 0, 0); } ;

  //expression
expression: '(' expression ')' { $$ = astCreate(AST_EXP_PARENTHESIS, 0, $2, 0, 0, 0); }
          | literal { $$ = $1; }
          | function_call { $$ = $1; }
		  		| vector_access { $$ = $1; }
          | identifier { $$ = $1; }
          | expression '+' expression { $$ = astCreate(AST_EXP_SUM, 0, $1, $3, 0, 0); }
          | expression '-' expression { $$ = astCreate(AST_EXP_SUB, 0, $1, $3, 0, 0); }
          | expression '*' expression { $$ = astCreate(AST_EXP_MUL, 0, $1, $3, 0, 0); }
          | expression '/' expression { $$ = astCreate(AST_EXP_DIV, 0, $1, $3, 0, 0); }
          | expression '%' expression { $$ = astCreate(AST_EXP_MOD, 0, $1, $3, 0, 0); }
          | expression '^' expression { $$ = astCreate(AST_EXP_POW, 0, $1, $3, 0, 0); }
          | expression '<' expression { $$ = astCreate(AST_EXP_LESS, 0, $1, $3, 0, 0); }
          | expression '>' expression { $$ = astCreate(AST_EXP_GREATER, 0, $1, $3, 0, 0); }
          | expression OPERATOR_LE expression { $$ = astCreate(AST_EXP_LE, 0, $1, $3, 0, 0); }
          | expression OPERATOR_GE expression { $$ = astCreate(AST_EXP_GE, 0, $1, $3, 0, 0); }
          | expression OPERATOR_EQ expression { $$ = astCreate(AST_EXP_EQ, 0, $1, $3, 0, 0); }
          | expression OPERATOR_DIF expression { $$ = astCreate(AST_EXP_DIF, 0, $1, $3, 0, 0); }
          | expression OPERATOR_AND expression { $$ = astCreate(AST_EXP_AND, 0, $1, $3, 0, 0); }
          | expression OPERATOR_OR expression { $$ = astCreate(AST_EXP_OR, 0, $1, $3, 0, 0); }
          | OPERATOR_NOT expression { $$ = astCreate(AST_EXP_NOT, 0, $2, 0, 0, 0); }
          ;

identifier: TK_IDENTIFIER { $$ = astCreate(AST_IDENTIFIER, $1, 0, 0, 0, 0); }
		  ;

%%

AST* getAST() {
    return root;
}

void yyerror(const char *msg) {
    printf("Error on line %d: %s\n", getLineNumber(), msg);
    exit(3);
}
