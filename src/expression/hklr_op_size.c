#include <assert.h>

#include "hkl_value.h"

HklValue* hklr_op_size(HklValue* value)
{
  // Dereference
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    hkl_value_free(temp);
  }

  switch (value->type)
  {
    case HKL_TYPE_STRING:
    {
      HklString* string = value->as.string;
      value->type = HKL_TYPE_INT;
      value->as.integer = string->length;
      hkl_string_free(string);
    }
    break;

    default:
      assert(false);
      break;
  }

  return value;
}