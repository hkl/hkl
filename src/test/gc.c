#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  HklString* index = hkl_string_new_from_utf8("index");
  HklString* hash = hkl_string_new_from_utf8("hash");
  HklString* ref = hkl_string_new_from_utf8("ref");


  // global index = "self"
  hklr_global_insert(
    index,
    hklr_object_new(HKL_TYPE_STRING, HKL_FLAG_NONE, hkl_string_new_from_utf8("self"))
  );

  // local hash = {}
  hklr_local_insert(
    hash,
    hklr_object_new(HKL_TYPE_HASH, HKL_FLAG_NONE, NULL)
  );

  // local ref = hash
  hklr_local_insert(
    ref,
    hklr_object_new(HKL_TYPE_REF, HKL_FLAG_NONE, hklr_search(hash))
  );

  // hash[index] = ref
  hklr_member_insert(hklr_search(hash),
                     hklr_search(index),
                     hklr_search(ref));

  hklr_shutdown();

  printf("Created: %zu\n", HKLR.gc_created);
  printf("Freed:   %zu\n", HKLR.gc_freed);
  printf("Cycles:  %zu\n", HKLR.gc_rootsize);
  printf("Runs:    %zu\n", HKLR.gc_runs);

  hkl_string_free(index);
  hkl_string_free(hash);
  hkl_string_free(ref);
}
