#include <assert.h>

#include "hklr.h"

#include "hkl_value.h"
#include "hkl_alloc.h"

HklValue* hkl_value_new(HklType type, ...)
{
  assert(type != HKL_TYPE_NONE);

  HklValue* value = hkl_alloc_object(HklValue);

  value->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_TYPE_INT:
      value->as.integer = va_arg(argp, int);
      break;

    case HKL_TYPE_TYPE:
      value->as.integer = va_arg(argp, HklType);
      break;

    case HKL_TYPE_REAL:
      value->as.real = va_arg(argp, double);
      break;

    case HKL_TYPE_STRING:
      value->as.string = va_arg(argp, HklString*);
      break;

    case HKL_TYPE_REF:
      value->as.object = va_arg(argp, HklrObject*);
      break;

    case HKL_TYPE_ARRAY:
      value->as.deque = va_arg(argp, HklDeque*);
      break;

    case HKL_TYPE_HASH:
      value->as.hash = va_arg(argp, HklHash*);
      break;

    case HKL_TYPE_FUNCTION:
      value->as.function = va_arg(argp, HklrFunction*);
      break;

    default:
      break;
  }

  va_end(argp);

  return value;
}

void hkl_value_free(HklValue* value)
{
  assert(value != NULL);

  switch (value->type)
  {
    case HKL_TYPE_STRING:
      hkl_string_free(value->as.string);
      break;

    case HKL_TYPE_ARRAY:
    {
      size_t i;

      for (i = 0; i < value->as.deque->size; ++i)
      {
        hkl_value_free((HklValue*) hkl_deque_findn(value->as.deque, i));
      }

      hkl_deque_free(value->as.deque);
    }
    break;

    case HKL_TYPE_HASH:
      hkl_hash_free(value->as.hash);
    break;

    default:
      break;
  }

  hkl_free_object(value);
}
