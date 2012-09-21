#include <stdio.h>
#include <string.h>

#include "hkl_string.h"
#include "hkl_tree.h"

typedef void(*testfunction)(const char**);

void hashtest(const char* argv[])
{
  printf("Hello World\n");
}

int main(int argc, const char* argv[])
{

  HklTree* testtree = hkl_tree_new();

  // Registered tests
  hkl_tree_insert(testtree, hkl_string_new_from_utf8("hashtest"), hashtest);






  testfunction test
    = hkl_tree_search(testtree, hkl_string_new_from_utf8(argv[1]))->value;

  if (test)
    test(&argv[2]);
  
  return 0;
}
