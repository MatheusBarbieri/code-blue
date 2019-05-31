#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE *yyin;

int main (int argc, char **argv)
{
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
		fprintf(stderr, "Success!\n");
		exit(0);
	}
}
