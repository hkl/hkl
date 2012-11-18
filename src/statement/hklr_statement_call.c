#include <assert.h>

#include "hklr_expression.h"
#include "hklr_statement.h"
#include "hkl_list.h"
#include "hklr.h"

static void hklr_statement_exec_list(void* stmt, void* data)
{
  hklr_statement_exec((HklrStatement*) stmt);
}

void hklr_statement_call(HklrExpression* expr, HklList* list)
{
  assert(expr != NULL);
  assert(list != NULL);

  HklValue* value = hklr_expression_eval(expr);
  assert(value != NULL);

  // dereference the objcet
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    hkl_value_free(temp);
  }

  // The calling object should be a function
  assert(value->type == HKL_TYPE_FUNCTION);

  HklrFunction* function = value->as.function;
  hkl_value_free(value);

  // execute the statements within
  hklr_scope_push();
  
  hkl_list_traverse(function->stmt_list, hklr_statement_exec_list, NULL);

  hklr_scope_pop();
}