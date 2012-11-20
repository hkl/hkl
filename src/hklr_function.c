#include <assert.h>

#include "hkl_alloc.h"
#include "hklr_function.h"
#include "hklr_statement.h"
#include "hklr_object.h"
#include "hklr.h"

static void process_closures(HklPair* pair, void* closure_list)
{
  // Search in our scope for the closed variables.
  HklrObject* object = hklr_exists(pair->key);

  if (object != NULL)
  {
    // increment the rc of the closed variable
    // we'll dec it when the function is destroyed
    hklr_gc_inc(object);
    pair->value = object;
  }
  else
  {
    pair->value = NULL;
  }
}

static void free_names(void* key, void* data)
{
  hkl_string_free((HklString*) key);
}

HklrFunction* hklr_function_new(HklList* args_list, HklTree* closure_list, HklList* stmt_list)
{
  HklrFunction* function = hkl_alloc_object(HklrFunction);

  function->stmt_list = stmt_list;
  function->args_list = args_list;
  function->closure_list = closure_list;

  hkl_tree_traverse(closure_list, process_closures, closure_list);

  // free the closure names, as we don't need them anymore

  return function;
}

static void dec_closures(HklPair* pair, void* data)
{
  if (pair->value != NULL)
    hklr_gc_dec((HklrObject*) pair->value);
}

static void free_statements(void* stmt, void* data)
{
  hklr_statement_free((HklrStatement*) stmt);
}

void hklr_function_free(HklrFunction* function)
{
  hkl_list_traverse(function->stmt_list, free_statements, NULL);
  hkl_list_free(function->stmt_list);

  hkl_list_traverse(function->args_list, free_names, NULL);
  hkl_list_free(function->args_list);

  hkl_tree_traverse(function->closure_list, dec_closures, NULL);
  hkl_tree_free(function->closure_list);

  hkl_free_object(function);
}
