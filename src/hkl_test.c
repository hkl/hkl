#include <stdio.h>
#include <string.h>

#include "hkl_string.h"
#include "hkl_hash.h"

int main(int argc, const char* argv[])
{
  HklHash* hash = hkl_hash_new();

  HklString* key = hkl_string_new_from_utf8("test");
  HklString* key1 = hkl_string_new_from_utf8("test1");
  HklString* key2 = hkl_string_new_from_utf8("test2");

  int i = 24;
  int j = 53;
  int k = 101;

  hkl_hash_insert(hash, key, &i);
  hkl_hash_insert(hash, key1, &j);
  hkl_hash_insert(hash, key2, &k);

  int* value = hkl_hash_search(hash, key)->value;
  int* value1 = hkl_hash_search(hash, key1)->value;
  int* value2 = hkl_hash_search(hash, key2)->value;
  
  printf("%d\n%d\n%d\n", *value, *value1, *value2);

  hkl_string_free(key);
  hkl_string_free(key1);
  hkl_string_free(key2);

  hkl_hash_free(hash);
  
  return 0;
}
