#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"

extern FILE *yyin;
void initMe(void);

int main (int argc, char **argv)
{
  	if (argc < 2) {
  		fprintf(stderr, "Call: a.out nome_do_arquivo\n");
  		exit(1);
  	}

  	if ((yyin = fopen(argv[1], "r")) == 0){
  		fprintf(stderr, "Nao foi possivel abrir o arquivo \%s\"\n", argv[1]);
  		exit(2);
  	}

  	initMe();

  	int result = yyparse();


  	if(result == 0){
          fprintf(stderr, "Arquivo aceito pela gramatica!\n");
          exit(0);
      }
}
