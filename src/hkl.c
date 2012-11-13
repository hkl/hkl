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
HklList* stmt_stack;
HklList* array_stack;
HklList* var_stack;

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  if (string_buf)
  fprintf(stderr, "On line %i: %s, read as \"%s\"\n", yylineno, msg, string_buf->utf8_data);
  else
  fprintf(stderr, "On line %i: %s\n", yylineno, msg);
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

  stmt_stack = hkl_list_new();
  array_stack = hkl_list_new();
  var_stack = hkl_list_new();

  yyparse();

  hklr_shutdown();
  
  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
