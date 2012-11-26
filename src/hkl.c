#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "hkl.h"
#include "linenoise/linenoise.h"

// These are linked at compile time
extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
extern HklString* string_buf;
extern char* yytext;
extern int yylineno;

bool interactive = false;
uint32_t qualifier_builder;
HklList* stmt_stack;
HklList* array_stack;
HklList* var_stack;
HklList* closure_stack;
HklList* id_stack;

typedef struct yy_buffer_state yy_buffer_state;
extern yy_buffer_state* yy_scan_string(const char*);
extern void yypop_buffer_state();
extern void yy_delete_buffer(yy_buffer_state*);

// This function is called when yyterminate is called.
// Which in turn is called when the end of input is reached.
// If we are in interactive mode, we do not want to end our parsing session.
char* line = NULL;
yy_buffer_state* parse_state = NULL;
int yywrap()
{
  if (interactive)
  {
    free(line);
    line = NULL;
    yy_delete_buffer(parse_state);
    parse_state = NULL;

    if (HKLR.scope_level > 0)
    {
      if ((line = linenoise("...> ")) != NULL)
      {  
        if (line[0] != '\0')
        {    
          parse_state = yy_scan_string(line);
          linenoiseHistoryAdd(line);
        }
        else free(line);
      }

      return false;
    }
  }

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

HklTree* keywords_map;
typedef struct comp_data
{
  const char* buf;
  size_t len;
  linenoiseCompletions* lc;

} comp_data;

#define MIN(a,b) (((a)<(b))?(a):(b))
bool keywords_traverse(HklPair* pair, void* data) {

  if (strncmp(((comp_data*) data)->buf, pair->key->utf8_data, MIN(((comp_data*) data)->len, pair->key->length))==0)
    linenoiseAddCompletion(((comp_data*) data)->lc, pair->key->utf8_data);

  return false;
}

bool vars_traverse(HklPair* pair, void* data) {

  if (strncmp(((comp_data*) data)->buf, pair->key->utf8_data, MIN(((comp_data*) data)->len, pair->key->length))==0)
    linenoiseAddCompletion(((comp_data*) data)->lc, pair->key->utf8_data);

  return false;
}

void completion(const char* buf, linenoiseCompletions* lc) {

  const char* truebuf = strrchr(buf, ' ');
  truebuf = (truebuf == NULL)? buf : truebuf + 1;

  if (truebuf[0] == '\0' || truebuf[0] == ' ')
    return;

  comp_data data = {truebuf, strlen(truebuf), lc};

  hkl_tree_traverse(keywords_map, keywords_traverse, &data);
  hkl_hash_traverse(((HklScope*)HKLR.scopes->tail->data)->locals, vars_traverse, &data);
  hkl_hash_traverse(HKLR.globals, vars_traverse, &data);
}

int main(int argc, const char* argv[])
{
  srand(time(NULL));

  keywords_map = hkl_tree_new();
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("if", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("while", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("end", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("include", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("nil", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("int", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("real", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("string", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("array", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("hash", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("func", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("type", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("true", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("false", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("maybe", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("puts", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("gets", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("hklr", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("collect", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("typeof", NULL));
  hkl_tree_move_pair(keywords_map, hkl_pair_new_from_utf8("function", NULL));

  // If there is a filename
  if (argv[1])
  {
    interactive = false;
    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
      fprintf(stderr, "Can't open: \"%s\"\n", argv[1]);
      return 1;
    }
  }
  else
  {
    interactive = true;
  }

  hklr_init();

  stmt_stack = hkl_list_new();
  array_stack = hkl_list_new();
  hkl_list_push_back(var_stack = hkl_list_new(), hkl_list_new());
  closure_stack = hkl_list_new();
  id_stack = hkl_list_new();

  // Parse files normally
  if (interactive == false)
  {
    if (yyparse())
      return 1;
  }
  // Drop into interactive mode with linenoise
  else
  {
    fprintf(stdout, "%s %s <%s>\n", PACKAGE_NAME, PACKAGE_VERSION, PACKAGE_URL);
    fprintf(stdout, "Use Ctrl-D (i.e. EOF) to exit\n");
    linenoiseSetCompletionCallback(completion);

    while((line = linenoise("hkl> ")) != NULL)
    {  
      yy_delete_buffer(parse_state);
      if (line[0] != '\0')
      {    
        parse_state = yy_scan_string(line);
        linenoiseHistoryAdd(line);
        if (yyparse())
        {
          return 1;
        }
      }
      else free(line);
    }

    yy_delete_buffer(parse_state);
    free(line);
  }

  hkl_list_free(stmt_stack);
  hkl_list_free(array_stack);
  hkl_list_free(hkl_list_pop_back(var_stack));
  hkl_list_free(var_stack);
  hkl_list_free(closure_stack);
  hkl_list_free(id_stack);

  hklr_shutdown();

  hkl_tree_free(keywords_map);
  
  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
