#include <assert.h>
#include <stdarg.h>

#include "hklr_object.h"
#include "hkl_alloc.h"
#include "hklr.h"

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

    default: 
      assert(false);
    break;
  }

  va_end(argp);

  return object;
}

void hklr_object_free(HklrObject* object)
{
  assert(object != NULL);

  switch (object->type)
  {
    case HKL_TYPE_STRING:
      hkl_string_free(object->as.string);
    break;

    default:

    break;
  }

  hkl_free_object(object);
}