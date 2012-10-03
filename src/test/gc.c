#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  // global index = "self"
  HklObject* index = hklr_object_new(HKL_TYPE_STRING, HKL_FLAG_NONE, hkl_string_new_from_utf8("self"));
  hkl_hash_insert(HKLR.globals, hkl_string_new_from_utf8("index"), index);

  // local hash = {}
  HklObject* hash = hklr_object_new(HKL_TYPE_HASH, HKL_FLAG_NONE, NULL);
  hkl_hash_insert(HKLR.scopes->locals, hkl_string_new_from_utf8("hash"), hash);

  // local ref = object
  HklObject* ref = hklr_object_new(HKL_TYPE_REF, HKL_FLAG_NONE, hash);
  hkl_hash_insert(HKLR.scopes->locals, hkl_string_new_from_utf8("ref"), ref);

  // hash[index] = ref
  hklr_member_insert(hash, index, ref);

  hklr_shutdown();

  printf("Created: %zu\n", HKLR.gc_created);
  printf("Freed:   %zu\n", HKLR.gc_freed);
  printf("Cycles:  %zu\n", HKLR.gc_rootsize);
  printf("Runs:    %zu\n", HKLR.gc_runs);
}
