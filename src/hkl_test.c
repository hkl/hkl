#include <stdio.h>
#include <string.h>

#include "hkl_string.h"
#include "hkl_hash.h"

int main(int argc, const char* argv[])
{
  HklHash *hash = hkl_hash_new();
  int someData = 24;
  HklString* str = hkl_string_new_from_utf8("hello");
  HklString* str2 = hkl_string_new_from_utf8("hello");

  hkl_hash_insert(hash, str,&someData);
  
  int result = *(int*)hkl_hash_find(hash,str2);
  
  printf("%d\n\n", result);
  return 0;
}
