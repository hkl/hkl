#ifndef HKL_VARIABLE_H
#define HKL_VARIABLE_H

#include <stdarg.h>

#include "hklr_expression.h"
#include "hkl_string.h"

typedef enum HklVariableType
{
  HKL_VAR_NONE,
  HKL_VAR_ID,
  HKL_VAR_INDEX,
  HKL_VAR_CALL

} HklVariableType;

typedef struct HklVariable
{
  HklVariableType type;

  union asvar {

    HklString* string;
    HklrExpression* expression;
    HklList* list;

  } as;

} HklVariable;

HklVariable* hkl_variable_new(HklVariableType type, ...);

void hkl_variable_free(HklVariable* variable);

#endif
