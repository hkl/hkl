#include <assert.h>

#include "hklr_expression.h"

void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs)
{
  // Evaluate the left hand side and then discard the value object
  HklValue* vobj = hklr_expression_eval(lhs);
  HklrObject* object = vobj->as.object;
  hkl_value_free(vobj);

  HklValue* value = hklr_expression_eval(rhs);

  assert(object != NULL);
  assert(value != NULL);

  // dereference the objcet
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    
    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY)
    {
      // simply spoof the value
      temp->type = HKL_TYPE_NIL;
    }
    
    hkl_value_free(temp);
  }

  // wipe out the original value

  // Composite objects will
  // create new objects to replace the variable
  // and decrease the ref count of the original
  switch (object->type)
  {
    case HKL_TYPE_STRING:
      hkl_string_free(object->as.string);
    break;

    default:
    break;
  }

  switch (value->type)
  {
    case HKL_TYPE_NIL:
      object->type = HKL_TYPE_NIL;
    break;

    case HKL_TYPE_INT:
      object->type = HKL_TYPE_INT;
      object->as.integer = value->as.integer;
    break;

    case HKL_TYPE_REAL:
      object->type = HKL_TYPE_REAL;
      object->as.real = value->as.real;
    break;

    case HKL_TYPE_STRING:
      object->type = HKL_TYPE_STRING;
      object->as.string = hkl_string_new_from_string(value->as.string);
    break;

    case HKL_TYPE_ARRAY:   

      // Make a reference to an array
      object->type = HKL_TYPE_REF;
      object->as.object = hklr_object_new(HKL_TYPE_ARRAY, HKL_FLAG_NONE, value->as.deque);
    break;

    default:
    assert(false);
    break;
  }

  // Be sure not to delete the deque
  if (value->type == HKL_TYPE_ARRAY)
  {
    value->type = HKL_TYPE_NIL;
  }

  hkl_value_free(value);
}
