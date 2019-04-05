#include <stdio.h>
#include "parser.tab.h"

int main (int argc, char **argv)
{
  return yyparse();
}
