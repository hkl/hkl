#include <stdio.h>

#include "hkl.h"

// These are linked at compile time
extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
extern int yylineno;

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  fprintf(stderr, "On line %i: %s\n", yylineno, msg);
  return true;
}

int main(int argc, const char* argv[])
{
  hklr_init();
  yyparse();

  hklr_shutdown();
  
  return 0;
}
