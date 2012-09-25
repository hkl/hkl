#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  HklString* self = hkl_string_new_from_utf8("self");

  // Create a hash table
  HklObject* object = hkl_object_new();
  HKLR.gc_created++;
  hklr_gc_inc(object);

  object->flags |= HKL_FLAG_HASH;
  object->hash = hkl_hash_new();

  // Create a reference to the hash table
  HklObject* ref = hkl_object_new();
  HKLR.gc_created++;
  hklr_gc_inc(ref);

  ref->flags |= HKL_FLAG_REF;
  ref->ref = object;
  hklr_gc_inc(object);

  // Insert the reference into the table
  hkl_hash_insert(object->hash, self, ref);
  hklr_gc_inc(ref);
  
  // End of stack. Free all locals
  hklr_gc_dec(object);
  hklr_gc_dec(ref);

  // A cycle exists. Comment out this line to see the difference.
  hklr_gc_collect();

  printf("Created: %zu\n", HKLR.gc_created);
  printf("Freed:   %zu\n", HKLR.gc_freed);
  printf("Cycles:  %zu\n", HKLR.gc_rootsize);
  printf("Runs:    %zu\n", HKLR.gc_runs);

  hklr_shutdown();
}
