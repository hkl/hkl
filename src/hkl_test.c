#include <stdio.h>

#include "hkl_tree.h"

typedef void(*HklTestFunction)(const char**);

int main(int argc, const char* argv[])
{

  HklTree* testtree = hkl_tree_new();

  // Registered tests

  // This line gives the function "hashtest" defined in test/hash.c
  // external linkage. If you don't know what that means, don't worry bout it.
  // :P :3
  extern HklTestFunction hashtest;
  hkl_tree_insert(testtree, hkl_string_new_from_utf8("hash"), &hashtest);

  extern HklTestFunction gctest;
  hkl_tree_insert(testtree, hkl_string_new_from_utf8("gc"), &gctest);

  if (argv[1] == NULL)
  {
    fprintf(stderr, "No test given. Exiting.\n");
    return 1;
  }

  HklPair* pair = hkl_tree_search(testtree, hkl_string_new_from_utf8(argv[1]));
  if (pair == NULL)
  {
    fprintf(stderr, "Test \"%s\" does not exist!\n", argv[1]);
    return 1;
  }
  
  HklTestFunction test = pair->value;

  if (test)
    test(&argv[2]);
  
  return 0;
}
