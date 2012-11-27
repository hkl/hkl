#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_variable.h"

HklVariable* hkl_variable_new(HklVariableType type, ...)
{
  HklVariable* variable = hkl_alloc_object(HklVariable);

  assert(type != HKL_VAR_NONE);
  variable->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_VAR_ID:
      variable->as.string = va_arg(argp, HklString*);
    break;

    case HKL_VAR_INDEX:
      variable->as.expression = va_arg(argp, HklrExpression*);
    break;

    default:
      assert(false);
    break;
  }

  va_end(argp);

  return variable;
}

void hkl_variable_free(HklVariable* variable)
{
  switch (variable->type)
  {
    case HKL_VAR_ID:
      hkl_string_free(variable->as.string);
    break;

    case HKL_VAR_INDEX:
      hklr_expression_free(variable->as.expression);
    break;

    default:
      assert(false);
    break;
  }

  hkl_free_object(variable);
}
