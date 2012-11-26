#include <assert.h>

#include "hklr_expression.h"
#include "hklr_statement.h"
#include "hkl_list.h"
#include "hklr.h"

static bool hklr_statement_exec_list(void* stmt, void* exec_code)
{
  *(bool*) exec_code = hklr_statement_exec((HklrStatement*) stmt);

  return *(bool*) exec_code;
}

void hklr_statement_while(HklrExpression* expr, HklList* list)
{
  assert(expr != NULL);
  assert(list != NULL);

  int exec_code = false;

  while (true)
  {
    HklValue* value = hklr_expression_eval(expr);
    bool test = false;

    assert(value != NULL);

    // dereference the objcet
    if (value->type == HKL_TYPE_REF)
    {
      HklValue* temp = value;
      value = hklr_object_dereference(value->as.object);
      hkl_value_free(temp);
    }

    switch (value->type)
    {
      case HKL_TYPE_INT:
        test = (value->as.integer)? true : false;
      break;

      case HKL_TYPE_REAL:
        test = (value->as.real)? true : false;
      break;

      case HKL_TYPE_STRING:
        test = (value->as.string->length)? true : false;
      break;

      default:
      break;
    }

    hkl_value_free(value);

    if (test == false)
      break;

    // execute the statements within
    hklr_scope_push();
    
    hkl_list_traverse(list, hklr_statement_exec_list, &exec_code);

    hklr_scope_pop();

    if (exec_code)
      break;
  }
}