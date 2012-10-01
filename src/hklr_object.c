#include <assert.h>

#include "hklr_object.h"
#include "hkl_alloc.h"
#include "hklr.h"

HklObject* hklr_object_new(HklTypes type, HklFlags flags, ...)
{
  assert(type != HKL_TYPE_NONE);

  HklObject* object = hkl_alloc_object(HklObject);

  object->prev = NULL;
  object->next = NULL;
  object->rc = 0;
  object->color = HKL_COLOR_BLACK;

  object->type = type;
  object->flags = flags;

  HKLR.gc_created++;
  object->flags = flags;
  object->type = type;
  hklr_gc_inc(object);

  va_list args;
  va_start(args, flags);

  switch (type)
  {
    case HKL_TYPE_HASH:
    {
      HklHash* hash = va_arg(args, HklHash*);

      if (hash != NULL)
      {
        object->as.hash = hash;
      }
      else 
      {
        object->as.hash = hkl_hash_new();
      }
      break;
    }

    case HKL_TYPE_REF:
    {
      HklObject* obj = va_arg(args, HklObject*);

      if (obj != NULL)
      {
        hklr_reference(object, obj);
      }
      else 
      {
        object->as.ref = NULL;
      }
      break;
    }

    case HKL_TYPE_STRING:
    {
      HklString* string = va_arg(args, HklString*);

      if (string != NULL)
      {
        object->as.string = string;
      }
      else 
      {
        object->as.string = hkl_string_new();
      }
      break;
    }

    default: break;
  }

  va_end(args);

  return object;
}

void hklr_object_free(HklObject* object)
{
  hkl_free_object(object);
}

void hklr_reference(HklObject* object, HklObject* reference)
{
  assert(object != NULL);
  assert(reference != NULL);

  assert(object->type == HKL_TYPE_REF);

  object->as.ref = reference;
  hklr_gc_inc(reference);
}

void hklr_member_insert(HklObject* object, HklObject* key, HklObject* value)
{
  assert(object != NULL);
  assert(key != NULL);
  assert(value != NULL);

  assert(object->type == HKL_TYPE_HASH);
  assert(key->type == HKL_TYPE_STRING);

  hkl_hash_insert(object->as.hash, key->as.string, value);
  hklr_gc_inc(value);
}
