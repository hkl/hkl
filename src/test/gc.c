#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  HklString* self = hkl_string_new_from_utf8("self");

  HklObject* object = hkl_object_new();
  hklr_gc_inc(object);

  object->flags |= HKL_FLAG_HASH;
  object->hash = hkl_hash_new();

  HklObject* ref = hkl_object_new();
  hklr_gc_inc(ref);

  ref->flags |= HKL_FLAG_REF;
  ref->ref = object;
  hklr_gc_inc(object);

  hkl_hash_insert(object->hash, self, ref);
  hklr_gc_inc(ref);
  
  hklr_gc_dec(object);
  hklr_gc_dec(ref);

  hklr_gc_collect();

  printf("%zu\n", HKLR.gc_freed);

  hklr_shutdown();
}