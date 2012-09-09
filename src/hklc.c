#include <stdio.h>

#include "hklc.h"

// These are linked at compile time
extern int yylex();
extern int yyparse();

int yywrap()
{
  return true;
}

int yyerror(char* msg)
{
  printf("Parsing error.\n");
  return true;
}

int main(int argc, const char* argv[])
{
  yyparse();
  
  return 0;
}
