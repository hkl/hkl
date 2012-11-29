#include <assert.h>

#include "hkl_alloc.h"
#include "hklr_function.h"
#include "hklr_statement.h"
#include "hklr_object.h"
#include "hklr.h"

static bool process_closures(HklPair* pair, void* data)
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

  return false;
}

static bool free_names(void* key, void* data)
{
  hkl_string_free((HklString*) key);

  return false;
}

/*static bool copy_statement_list(void* statement, void* list)
{
  hkl_list_push_back((HklList*) list, hklr_statement_new_from_statement((HklrStatement*) statement));

  return false;
}

static bool copy_args_list(void* arg, void* list)
{
  hkl_list_push_back((HklList*) list, hkl_string_new_from_string((HklString*) arg));

  return false;
}*/

HklrFunction* hklr_function_new(HklList* args_list, HklTree* closure_list, HklList* stmt_list)
{
  HklrFunction* function = hkl_alloc_object(HklrFunction);

  function->stmt_list = stmt_list;
  //hkl_list_traverse(stmt_list, copy_statement_list, function->stmt_list);

  function->args_list = args_list;
  function->closure_list = closure_list;

  hkl_tree_traverse(closure_list, process_closures, NULL);

  return function;
}

static bool dec_closures(HklPair* pair, void* data)
{
  if (pair->value != NULL)
    hklr_gc_dec((HklrObject*) pair->value);

  return false;
}

static bool free_statements(void* stmt, void* data)
{
  hklr_statement_free((HklrStatement*) stmt);

  return false;
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
