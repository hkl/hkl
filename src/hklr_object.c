#include <assert.h>
#include <stdarg.h>

#include "hklr_object.h"
#include "hkl_alloc.h"
#include "hklr.h"
#include "hkl_deque.h"
#include "hkl_value.h"
#include "hklr_function.h"
#include "hklr_expression.h"

HklrObject* hklr_object_new(HklType type, HklFlag flags, ...)
{
  assert(type != HKL_TYPE_NONE);

  HklrObject* object = hkl_alloc_object(HklrObject);

  object->prev = NULL;
  object->next = NULL;
  object->rc = 0;
  object->is_buffered = false;
  object->color = HKL_COLOR_BLACK;

  object->type = type;
  object->flags = flags;

  HKLR.gc_created++;
  object->flags = flags;
  object->type = type;
  hklr_gc_inc(object);

  va_list argp;
  va_start(argp, flags);

  //void* pointer = NULL;
  switch (type)
  {
    case HKL_TYPE_NIL:
    break;

    case HKL_TYPE_INT:
      object->as.integer = va_arg(argp, int);
    break;

    case HKL_TYPE_REAL:
      object->as.real = va_arg(argp, double);
    break;

    case HKL_TYPE_STRING:
      object->as.string = va_arg(argp, HklString*);
    break;

    case HKL_TYPE_ARRAY:
      object->as.deque = va_arg(argp, HklDeque*);
    break;

    case HKL_TYPE_HASH:
      object->as.hash = va_arg(argp, HklHash*);
    break;

    case HKL_TYPE_FUNCTION:
      object->as.function = va_arg(argp, HklrFunction*);
    break;

    default: 
      assert(false);
    break;
  }

  va_end(argp);

  return object;
}

HklValue* hklr_object_dereference(HklrObject* object)
{
  assert(object != NULL);

  switch (object->type)
  {
    case HKL_TYPE_NIL:
      return hkl_value_new(HKL_TYPE_NIL);
    break;

    case HKL_TYPE_INT:
      return hkl_value_new(HKL_TYPE_INT, object->as.integer);
    break;

    case HKL_TYPE_TYPE:
      return hkl_value_new(HKL_TYPE_TYPE, object->as.type);
    break;

    case HKL_TYPE_REAL:
      return hkl_value_new(HKL_TYPE_REAL, object->as.real);
    break;

    case HKL_TYPE_STRING:
      return hkl_value_new(HKL_TYPE_STRING, hkl_string_new_from_string(object->as.string));
    break;    

    case HKL_TYPE_ARRAY:
      return hkl_value_new(HKL_TYPE_ARRAY, object->as.deque);
    break;

    case HKL_TYPE_HASH:
      return hkl_value_new(HKL_TYPE_HASH, object->as.hash);
    break;

    case HKL_TYPE_FUNCTION:
      return hkl_value_new(HKL_TYPE_FUNCTION, object->as.function);
    break;
    
    // A composite object reference
    case HKL_TYPE_REF:
      return hklr_object_dereference(object->as.object);
    break;     

    default:
      assert(false);
      return NULL;
    break;
  }
}

void hklr_object_assign(HklrObject* object, HklrExpression* expression)
{
  HklValue* value = hklr_expression_eval(expression);

  assert(object != NULL);
  assert(value != NULL);

  // The right hand object's compositeness
  bool composite = false;

  HklValue* temp = value;
  // dereference the object
  if (value->type == HKL_TYPE_REF)
  {
    value = hklr_object_dereference(value->as.object);

    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY || value->type == HKL_TYPE_HASH || value->type == HKL_TYPE_FUNCTION)
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
        // Double reference (composite types)
        case HKL_TYPE_ARRAY:
        case HKL_TYPE_FUNCTION:
        case HKL_TYPE_HASH:
          // dec the rc of the object
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
    case HKL_TYPE_FUNCTION:
    case HKL_TYPE_HASH:

      // Make a reference
      object->type = HKL_TYPE_REF;

      if (composite)
      {
        object->as.object = temp->as.object->as.object;
        hkl_value_free(temp); // free the temp rhs
      }
      else if (value->type == HKL_TYPE_ARRAY)
      {
        object->as.object = hklr_object_new(HKL_TYPE_ARRAY, HKL_FLAG_NONE, value->as.deque);
      }
      else if (value->type == HKL_TYPE_HASH)
      {
        object->as.object = hklr_object_new(HKL_TYPE_HASH, HKL_FLAG_NONE, value->as.hash);
      }
      else if (value->type == HKL_TYPE_FUNCTION)
      {
        object->as.object = hklr_object_new(HKL_TYPE_FUNCTION, HKL_FLAG_NONE, value->as.function);
      }

    break;

    default:
    assert(false);
    break;
  }

  // Be sure not to delete composties
  if (value->type == HKL_TYPE_ARRAY || value->type == HKL_TYPE_HASH || value->type == HKL_TYPE_FUNCTION)
  {
    value->type = HKL_TYPE_NIL;
  }

  hkl_value_free(value);
}


void hklr_object_free(HklrObject* object)
{
  assert(object != NULL);

  switch (object->type)
  {
    case HKL_TYPE_STRING:
      hkl_string_free(object->as.string);
    break;

    case HKL_TYPE_ARRAY:
    {
      size_t i;

      for (i = 0; i < object->as.deque->size; ++i)
      {
        hkl_value_free((HklValue*) hkl_deque_findn(object->as.deque, i));
      }

      hkl_deque_free(object->as.deque);
    }
    break;

    case HKL_TYPE_HASH:
      hkl_hash_free(object->as.hash);
    break;

    case HKL_TYPE_FUNCTION:
      hklr_function_free(object->as.function);
    break;

    default:

    break;
  }

  hkl_free_object(object);
}
