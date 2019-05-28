#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE *yyin;
void initMe(void);

int main (int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Error 1. Must call: a.out file_name\n");
		exit(1);
	}

	if ((yyin = fopen(argv[1], "r")) == 0){
		fprintf(stderr, "Error 2. Cannot open file. \%s\"\n", argv[1]);
		exit(2);
	}

	initMe();

	int result = yyparse();

	if(result == 0){
		fprintf(stderr, "Success!\n");
		exit(0);
	}
}
