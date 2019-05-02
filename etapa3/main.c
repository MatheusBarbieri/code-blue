#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "ast.h"

extern FILE *yyin;
void initMe(void);
AST* getAST();
void astToFile();

int main (int argc, char **argv)
{
	FILE *outputFile;

	if (argc < 3) {
		fprintf(stderr, "Error 1. Must call: ./etapa3 inputfile outputfile\n");
		exit(1);
	}

	if ((yyin = fopen(argv[1], "r")) == 0){
		fprintf(stderr, "Error 2. Cannot open file. %s\n", argv[1]);
		exit(2);
	}

	if ((outputFile = fopen(argv[2], "w")) == 0) {
		fprintf(stderr, "Error 3. Cannot create file %s\n", argv[2]);
		exit(3);
	}

	int result = yyparse();

	AST* ast = getAST();
	astToFile(ast, outputFile);

	if(result == 0){
		exit(0);
	}
}
