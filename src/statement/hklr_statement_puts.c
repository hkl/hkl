#include <assert.h>

#include "hklr_expression.h"

void hklr_statement_puts(HklrExpression* expr)
{
  assert(expr != NULL);

  HklValue* value = hklr_expression_eval(expr);

  // derefence the object
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    hkl_value_free(temp);
  }

  switch (value->type)
  {
    case HKL_TYPE_NIL:
      fprintf(stdout, "nil");
      break;

    case HKL_TYPE_INT:
      fprintf(stdout, "%i", value->as.integer);
      break;

    case HKL_TYPE_REAL:
      fprintf(stdout, "%lg", value->as.real);
      break;

    case HKL_TYPE_STRING:
      fprintf(stdout, "%s", value->as.string->utf8_data);
      break;

    default:
      assert(false);
      break;
  }

  // flush the output
  fflush(stdout);

  hkl_value_free(value);
}