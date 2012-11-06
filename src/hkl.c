#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "hkl.h"

// These are linked at compile time
extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
extern HklString* string_buf;
extern char* yytext;
extern int yylineno;

uint32_t qualifier_builder;

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  if (string_buf)
  fprintf(stderr, "On line %i: %s, read as \"%s\"\n", yylineno, msg, string_buf->utf8_data);
  else
  fprintf(stderr, "On line %i: %s, read as \"%s\"\n", yylineno, msg, yytext);
  return true;
}

int main(int argc, const char* argv[])
{
  srand(time(NULL));

  // If there is a filename
  if (argv[1])
  {
    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
      fprintf(stderr, "Can't open: \"%s\"\n", argv[1]);
      return 1;
    }
  }

  hklr_init();
  yyparse();

  hklr_shutdown();
  
  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
