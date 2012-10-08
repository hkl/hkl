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
