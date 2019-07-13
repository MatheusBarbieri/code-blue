#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "tacs.h"


extern FILE *yyin;

void checkUndeclared();
void setDeclaration(AST* node);
AST* getAST(void);
extern TAC * getTACs();


int main (int argc, char **argv)
{
	FILE *outputFile = fopen(argv[2], "w");
	if (argc < 2) {
		fprintf(stderr, "[Error] Missing file path. Must call: ./etapa4 <input_file_path>\n");
		exit(1);
	}

	if ((yyin = fopen(argv[1], "r")) == 0){
		fprintf(stderr, "[Error] Cannot open file: %s\n", argv[1]);
		exit(2);
	}

	int result = yyparse();

	if(result == 0){
		fprintf(stderr, "Hash table of the input file: \n");
		hashPrint();
		fprintf(stderr, "\nSyntactical tree for the input file: \n");
		astPrint(0, getAST());
		astToFile(getAST(), outputFile);

		fprintf(stderr, "\nSource code OK!\n");

		// checkUndeclared();
		// setDeclaration(getAST());
		printAllTacs(getTACs());

		fprintf(stderr, "\nNo semantic errors!\n");


		exit(0);
	}
}
