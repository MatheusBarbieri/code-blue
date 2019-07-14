#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "tacs.h"


extern FILE *yyin;

AST* getAST(void);

int main (int argc, char **argv)
{
	if ((yyin = fopen(argv[1], "r")) == 0){
		fprintf(stderr, "[Error] Cannot open file: %s\n", argv[1]);
		exit(2);
	}

	int result = yyparse();

	if(result == 0){
		AST* ast = getAST();

		fprintf(stderr, "=============================\n");
		fprintf(stderr, "Syntactical tree for the input file:\n");
		astPrint(0, ast);

		fprintf(stderr, "\n=============================\n");
		fprintf(stderr, "Three Adress Codes:\n");
		tacPrintForward(indexReverseOrder(tacGenerate(ast, NULL)));

		fprintf(stderr, "\n=============================\n");
		fprintf(stderr, "Hash table after TACs: \n");
		hashPrint();
		fprintf(stderr, "\n=============================\n");

		exit(0);
	}
}
