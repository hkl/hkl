#include <assert.h>

#include "hkl_value.h"

HklValue* hklr_op_less_equal(HklValue* left_value, HklValue* right_value)
{
  HklValue* result = NULL;
  bool left_updated = false, right_updated = false;

  // Dereference left and right sides
  if (left_value->type == HKL_TYPE_REF)
  {
    left_updated = true;
    left_value = hklr_object_dereference(left_value->as.object);
  }

  if (right_value->type == HKL_TYPE_REF)
  {
    right_updated = true;
    right_value = hklr_object_dereference(right_value->as.object);
  }

  switch (left_value->type)
  {
    case HKL_TYPE_INT:
      switch(right_value->type)
      {
        case HKL_TYPE_INT:
          result = hkl_value_new(HKL_TYPE_INT,
            left_value->as.integer <= right_value->as.integer);
          break;

        case HKL_TYPE_REAL:
          result = hkl_value_new(HKL_TYPE_INT,
            left_value->as.integer <= right_value->as.real);
          break;

        default:
          assert(false);
          break;
      }
      break; // HKL_TYPE_INT

    case HKL_TYPE_REAL:
      switch(right_value->type)
      {
        case HKL_TYPE_INT:
          result = hkl_value_new(HKL_TYPE_INT,
            left_value->as.real <= right_value->as.integer);
          break;

        case HKL_TYPE_REAL:
          result = hkl_value_new(HKL_TYPE_INT,
            left_value->as.real <= right_value->as.real);
          break;

        default:
          assert(false);
          break;
      }
      break; // HKL_TYPE_REAL
    
    case HKL_TYPE_STRING:
      switch(right_value->type)
      {
        case HKL_TYPE_STRING:  
          result = hkl_value_new(HKL_TYPE_INT,
            hkl_string_compare(left_value->as.string,
                               right_value->as.string) <= 0);
          break;

        default:
          assert(false);
          break;
      }
      break; // HKL_TYPE_STRING

    default:
      assert(false);
      break;
  }

  if (left_updated)
  {
    hkl_value_free(left_value);
  }

  if (right_updated)
  {
    hkl_value_free(right_value);
  }

  return result;
}