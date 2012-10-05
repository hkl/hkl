#include <stdio.h>

#include "../hklr.h"

void gctest(const char* argv[])
{
  hklr_init();

  // global index = "self"
  hklr_global_insert(
    hkl_string_new_from_utf8("index"),
    hklr_object_new(HKL_TYPE_STRING, HKL_FLAG_NONE, hkl_string_new_from_utf8("self"))
  );

  // local hash = {}
  hklr_local_insert(
    hkl_string_new_from_utf8("hash"),
    hklr_object_new(HKL_TYPE_HASH, HKL_FLAG_NONE, NULL)
  );

  // local ref = hash
  hklr_local_insert(
    hkl_string_new_from_utf8("ref"),
    hklr_object_new(HKL_TYPE_REF, HKL_FLAG_NONE, hklr_search(hkl_string_new_from_utf8("hash")))
  );

  // hash[index] = ref
  hklr_member_insert(hklr_search(hkl_string_new_from_utf8("hash")),
                     hklr_search(hkl_string_new_from_utf8("index")),
                     hklr_search(hkl_string_new_from_utf8("ref")));

  hklr_shutdown();

  printf("Created: %zu\n", HKLR.gc_created);
  printf("Freed:   %zu\n", HKLR.gc_freed);
  printf("Cycles:  %zu\n", HKLR.gc_rootsize);
  printf("Runs:    %zu\n", HKLR.gc_runs);
}
