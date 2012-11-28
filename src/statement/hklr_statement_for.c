#include <assert.h>

#include "hklr_expression.h"
#include "hklr_statement.h"
#include "hkl_list.h"
#include "hklr.h"

extern void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs);

static void exec_for_loop(void* stmt, void* data)
{
  hklr_statement_exec((HklrStatement*) stmt);
}

void hklr_statement_for(HklString* id, HklrExpression* expr, HklList* list)
{
  assert(expr != NULL);
  assert(list != NULL);

  HklValue* value = hklr_expression_eval(expr);

  assert(value != NULL);

  // dereference the object
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    hkl_value_free(temp);
  }

  assert(value->type == HKL_TYPE_ARRAY);

  hkl_value_free(value);

  // Create the variable expression for the iterator
  HklrExpression* iter = hklr_expression_new(HKL_EXPR_VAR, hkl_string_new_from_string((HklString*) id), hkl_list_new());
  HklrExpression* i = hklr_expression_new(HKL_EXPR_INT, 0);

  for (i->arg[0].integer = 0; (size_t) i->arg[0].integer < value->as.deque->size; ++i->arg[0].integer)
  {
    hklr_scope_push();

    // assign the iter
    hklr_statement_assign(iter, i);

    // execute the statements within
    hkl_list_traverse(list, exec_for_loop, NULL);

    hklr_scope_pop();
  }

  hklr_expression_free(iter);
}