#include <assert.h>

#include "hkl_value.h"
#include "hkl_string.h"

// Make a new value object that is a cast of the value as a int
HklValue* hklr_as_real(HklValue* value)
{
  HklValue* cast = hkl_value_new(HKL_TYPE_REAL, NULL);

  switch (value->type)
  {
    case HKL_TYPE_REF:
    {
      HklValue* temp = hklr_object_dereference(value->as.object);
      cast->as.real = hklr_as_real(temp)->as.real;
    }
    break;

    case HKL_TYPE_NIL:
      cast->as.real = 0.0;
      break;

    case HKL_TYPE_INT:
      cast->as.real = (double) value->as.integer;
      break;

    case HKL_TYPE_REAL:
      cast->as.real = value->as.real;
      break;

    case HKL_TYPE_STRING:
      sscanf(value->as.string->utf8_data, "%lg", &cast->as.real);
      break;

    default:
      assert(false);
      break;
  }

  return cast;
}