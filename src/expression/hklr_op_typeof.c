#include <assert.h>

#include "hkl_value.h"

HklValue* hklr_op_typeof(HklValue* value)
{
  bool temporary = true;

  // Dereference
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    
    value = hklr_object_dereference(value->as.object);
    
    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY || value->type == HKL_TYPE_HASH || value->type == HKL_TYPE_FUNCTION)
    {
      temporary = false;
      // simply spoof the value
      temp->type = HKL_TYPE_NIL;
    }

    hkl_value_free(temp);
  }

  HklType type = value->type;

  // Spoof the value if it not temporary
  if (temporary == false)
    value->type = HKL_TYPE_NIL;

  hkl_value_free(value);

  value = hkl_value_new(HKL_TYPE_TYPE, type);
  value->type = HKL_TYPE_TYPE;
  return value;
}