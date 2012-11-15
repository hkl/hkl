#include <assert.h>

#include "hkl_value.h"

HklValue* hklr_op_size(HklValue* value)
{

  bool temporary = true;

  // Dereference
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    
    value = hklr_object_dereference(value->as.object);
    
    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY)
    {
      temporary = false;
      // simply spoof the value
      temp->type = HKL_TYPE_NIL;
    }

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

    case HKL_TYPE_ARRAY:
    {
      HklDeque* deque = value->as.deque;
      value->type = HKL_TYPE_INT;
      value->as.integer = deque->size;

      if (temporary)
      {
        // Free the deque
        hkl_value_free(hkl_value_new(HKL_TYPE_ARRAY, deque));
      }
    }
    break;

    default:
      assert(false);
      break;
  }

  return value;
}