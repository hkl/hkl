#include <assert.h>

#include "hkl_value.h"
#include "hkl_string.h"

// Make a new value object that is a cast of the value as a int
HklValue* hklr_as_integer(HklValue* value)
{
  HklValue* cast = hkl_value_new(HKL_TYPE_INT, NULL);

  switch (value->type)
  {
    case HKL_TYPE_REF:
    {
      HklValue* temp = hklr_object_dereference(value->as.object);
      cast->as.integer = hklr_as_integer(temp)->as.integer;
    }
    break;

    case HKL_TYPE_NIL:
      cast->as.integer = 0;
      break;

    case HKL_TYPE_INT:
      cast->as.integer = value->as.integer;
      break;

    case HKL_TYPE_REAL:
      cast->as.integer = (int) value->as.real;
      break;

    case HKL_TYPE_STRING:
      sscanf(value->as.string->utf8_data, "%i", &cast->as.integer);
      break;

    default:
      assert(false);
      break;
  }

  return cast;
}