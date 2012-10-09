#include <stdio.h>

#include "../hkl_hash.h"

void print(HklPair* pair, void* ptr){
  printf("VALUE: %s \n", hkl_string_get_utf8(pair->key));
}

void traversaltest(const char* argv[])
{
  HklString* k1 = hkl_string_new_from_utf8("sometestkey 13");
  HklString* k2 = hkl_string_new_from_utf8("jargon 45");
  HklString* k3 = hkl_string_new_from_utf8("blah 78 100");
  HklString* k4 = hkl_string_new_from_utf8("hello world 19");

  int val1 = 13;
  int val2 = 45;
  int val3 = 78;
  int val4 = 100;
  int val5 = 19;

  HklHash* hash = hkl_hash_new();

  hkl_hash_insert(hash, k1, &val1);
  hkl_hash_insert(hash, k2, &val2);
  hkl_hash_insert(hash, k3, &val3);
  hkl_hash_insert(hash, k3, &val4);
  hkl_hash_insert(hash, k4, &val5);

  hkl_hash_traverse(hash, print, NULL);
  
  hkl_hash_traverse(hash, print, NULL);

  hkl_string_free(k1);
  hkl_string_free(k2);
  hkl_string_free(k3);
  hkl_string_free(k4);

  hkl_hash_free(hash);
}
