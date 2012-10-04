#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  // Create a string "self"
  HklObject* self = hklr_object_new(HKL_TYPE_STRING, HKL_FLAG_NONE, hkl_string_new_from_utf8("self"));

  // Create a hash table
  HklObject* object = hklr_object_new(HKL_TYPE_HASH, HKL_FLAG_NONE, NULL);

  // Create a reference to the hash table
  HklObject* ref = hklr_object_new(HKL_TYPE_REF, HKL_FLAG_NONE, object);

  // Insert the reference into the table
  hklr_member_insert(object, self, ref);
  
  // End of stack. Free all locals
  hklr_gc_dec(object);
  hklr_gc_dec(ref);
  hklr_gc_dec(self);

  // A cycle exists. Comment out this line to see the difference.
  hklr_gc_collect();

  hklr_shutdown();

  printf("Created: %zu\n", HKLR.gc_created);
  printf("Freed:   %zu\n", HKLR.gc_freed);
  printf("Cycles:  %zu\n", HKLR.gc_rootsize);
  printf("Runs:    %zu\n", HKLR.gc_runs);
}
