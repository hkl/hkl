#include <assert.h>

#include "hkl_alloc.h"
#include "hklr_function.h"
#include "hklr_statement.h"
#include "hklr_object.h"
#include "hklr.h"

static void process_closures(void* key, void* closure_list)
{
  // Search in our scope for the closed variables.
  HklrObject* object = hklr_search((HklString*) key);

  // increment the rc of the closed variable
  // we'll dec it when the function is destroyed
  hklr_gc_inc(object);

  // Add that ref object to the closure list local to the function
  hkl_list_push_back((HklList*) closure_list, object);
}

static void free_names(void* key, void* data)
{
  hkl_string_free((HklString*) key);
}

HklrFunction* hklr_function_new(HklList* args_list, HklList* closure_list, HklList* stmt_list)
{
  HklrFunction* function = hkl_alloc_object(HklrFunction);

  function->stmt_list = stmt_list;
  function->args_list = args_list;
  function->closure_list = hkl_list_new();

  hkl_list_traverse(closure_list, process_closures, function->closure_list);

  // free the closure names, as we don't need them anymore
  hkl_list_traverse(closure_list, free_names, NULL);
  hkl_list_free(closure_list);

  return function;
}

static void dec_closures(void* object, void* data)
{
  hklr_gc_dec((HklrObject*) object);
}

static void free_statements(void* stmt, void* data)
{
  hklr_statement_free((HklrStatement*) stmt);
}

void hklr_function_free(HklrFunction* function)
{
  hkl_list_traverse(function->stmt_list, free_statements, NULL);
  hkl_list_free(function->stmt_list);

  hkl_list_traverse(function->closure_list, dec_closures, NULL);
  hkl_list_free(function->closure_list);

  hkl_free_object(function);
}