#include <assert.h>

#include "hklr_common.h"
#include "hkl_hash.h"
#include "hkl_string.h"

HklObject* hklr_create(HklTypes type, HklFlags flags, void* value)
{
  assert(type != HKL_TYPE_NONE);

  HklObject* object = hkl_object_new();
  HKLR.gc_created++;
  object->flags = flags;
  object->type = type;
  hklr_gc_inc(object);

  switch (type)
  {
    case HKL_TYPE_HASH:
      if (value != NULL)
      {
        object->hash = value;
      }
      else 
      {
        object->hash = hkl_hash_new();
      }
    break;

    case HKL_TYPE_REF:
      if (value != NULL)
      {
        hklr_reference(object, value);
      }
      else 
      {
        object->ref = NULL;
      }
    break;

    case HKL_TYPE_STRING:
      if (value != NULL)
      {
        object->string = value;
      }
      else 
      {
        object->string = hkl_string_new();
      }
    break;

    default: assert(false); break;
  }

  return object;
}

void hklr_reference(HklObject* object, HklObject* reference)
{
  assert(object != NULL);
  assert(reference != NULL);

  assert(object->type == HKL_TYPE_REF);

  object->ref = reference;
  hklr_gc_inc(reference);
}

void hklr_member_insert(HklObject* object, HklObject* key, HklObject* value)
{
  assert(object != NULL);
  assert(key != NULL);
  assert(value != NULL);

  assert(object->type == HKL_TYPE_HASH);
  assert(key->type == HKL_TYPE_STRING);

  hkl_hash_insert(object->hash, key->string, value);
  hklr_gc_inc(value);
}