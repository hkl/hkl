#include <assert.h>

#include "hklr_object.h"
#include "hkl_alloc.h"
#include "hklr.h"

HklObject* hklr_object_new(HklType type, HklFlag flags, void* value)
{
  assert(type != HKL_TYPE_NONE);

  HklObject* object = hkl_alloc_object(HklObject);

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

  switch (type)
  {
    case HKL_TYPE_HASH:

      if (value != NULL)
      {
        object->as.hash = value;
      }
      else 
      {
        object->as.hash = hkl_hash_new();
      }
      break;

    case HKL_TYPE_REF:

      if (value != NULL)
      {
        assert(((HklObject*) value)->type != HKL_TYPE_REF);
        hklr_reference(object, value);
      }
      else 
      {
        object->as.ref = NULL;
      }
      break;

    case HKL_TYPE_STRING:

      if (value != NULL)
      {
        object->as.string = value;
      }
      else 
      {
        object->as.string = hkl_string_new();
      }
      break;

    default: break;
  }

  return object;
}

void hklr_object_free(HklObject* object)
{
  assert(object != NULL);

  switch (object->type)
  {
    case HKL_TYPE_HASH:
      hkl_hash_free(object->as.hash);
      break;

    case HKL_TYPE_REF:
      // dont free the thing im referencing
      break;

    case HKL_TYPE_STRING:
      hkl_string_free(object->as.string);
      break;

    default: break;
  }

  hkl_free_object(object);
}

void hklr_reference(HklObject* object, HklObject* reference)
{
  assert(object != NULL);
  assert(reference != NULL);

  assert(object->type == HKL_TYPE_REF);

  // if the other object is a reference, we want to reference what it
  // is referencing

  if (reference->type == HKL_TYPE_REF)
  {
    object->as.ref = reference->as.ref;
    hklr_gc_inc(reference->as.ref);
  }
  else
  {
    object->as.ref = reference;
    hklr_gc_inc(reference);    
  }
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
