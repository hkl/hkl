#include <assert.h>

#include "hkl_value.h"

HklValue* hklr_op_coalesce(HklValue* left_value, HklValue* right_value)
{
  HklValue* result = right_value;

  if (left_value->type != HKL_TYPE_NIL)
  {
    if (left_value->type != HKL_TYPE_REF)
    {
      result = left_value;
    }
    else if (left_value->as.object->type != HKL_TYPE_NIL)
    {
      if (left_value->as.object->type != HKL_TYPE_REF)
      {
        result = left_value;
      }
      else if (left_value->as.object->as.object->type != HKL_TYPE_NIL)
      {
        result = left_value;
      }
    }
  }

  return result;
}