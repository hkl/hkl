#include <stdio.h>

#include "hkl_tree.h"

typedef void(*HklTestFunction)(const char**);

int main(int argc, const char* argv[])
{

  HklTree* testtree = hkl_tree_new();
  HklString* testname = hkl_string_new();

  // Registered tests

  // This line gives the function "hashtest" defined in test/hash.c
  // external linkage. If you don't know what that means, don't worry bout it.
  // :P :3
  extern HklTestFunction hashtest;
  hkl_string_set_utf8(testname, "hash");
  hkl_tree_insert(testtree, testname, &hashtest);

  extern HklTestFunction gctest;
  hkl_string_set_utf8(testname, "gc");
  hkl_tree_insert(testtree, testname, &gctest);

  if (argv[1] == NULL)
  {
    fprintf(stderr, "No test given. Exiting.\n");
    return 1;
  }

  hkl_string_set_utf8(testname, argv[1]);
  HklPair* pair = hkl_tree_search(testtree, testname);
  if (pair == NULL)
  {
    fprintf(stderr, "Test \"%s\" does not exist!\n", argv[1]);
    return 1;
  }
  
  HklTestFunction test = pair->value;

  if (test)
    test(&argv[2]);

  hkl_string_free(testname);
  hkl_tree_free(testtree);
  
  return 0;
}
