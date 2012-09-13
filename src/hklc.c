#include <stdio.h>

#include "hklc.h"

// These are linked at compile time
extern int yylex();
extern int yyparse();
extern int yylineno;

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  printf("On line %i: %s\n", yylineno, msg);
  return true;
}

int main(int argc, const char* argv[])
{
  yyparse();
  
  return 0;
}
