#include <stdio.h>

#include "hklc.h"

// These are linked at compile time
extern int yylex();
extern int yyparse();

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  printf("Parsing error: %s\n", msg);
  return true;
}

int main(int argc, const char* argv[])
{
  yyparse();
  
  return 0;
}
