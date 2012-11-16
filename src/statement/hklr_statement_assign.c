#include <assert.h>

#include "hklr.h"
#include "hklr_expression.h"

void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs)
{
  // Evaluate the left hand side
  HklValue* vobj = hklr_expression_eval(lhs);
  HklrObject* object = vobj->as.object;
  hkl_value_free(vobj);

  HklValue* value = hklr_expression_eval(rhs);

  assert(object != NULL);
  assert(value != NULL);

  // The right hand object's compositeness
  bool composite = false;

  HklValue* temp = value;
  // dereference the objcet
  if (value->type == HKL_TYPE_REF)
  {
    value = hklr_object_dereference(value->as.object);
    
    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY)
    {
      // The right hand side is a composite
      composite = true;
      // increase its ref count since its about to be ref'ed
      hklr_gc_inc(temp->as.object->as.object);
    }
    
    if (composite == false)
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

    case HKL_TYPE_REF:
      switch(object->as.object->type)
      {
        // Double reference (Arrays and Hashes)
        case HKL_TYPE_ARRAY:
          // dec the rc of the array
          hklr_gc_dec(object->as.object);
        break;

        default:
          assert(false);
        break;
      }
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

    case HKL_TYPE_TYPE:
      object->type = HKL_TYPE_TYPE;
      object->as.type = value->as.type;
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

      if (composite)
      {
        object->as.object = temp->as.object->as.object;
        hkl_value_free(temp); // free the temp rhsma
      }
      else
      {
        object->as.object = hklr_object_new(HKL_TYPE_ARRAY, HKL_FLAG_NONE, value->as.deque);
      }

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
