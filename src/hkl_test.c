#include <stdio.h>
#include <string.h>

#include "hkl_string.h"

int main(int argc, const char* argv[])
{
  HklString* runes = hkl_string_new_from_utf8("ᛖᚴ ᚷᛖᛏ ᛖᛏᛁ ᚧ ᚷᛚᛖᚱ ᛘᚾ ᚦᛖᛋᛋ ᚨᚧ ᚡᛖ ᚱᚧᚨ ᛋᚨᚱ");
  HklString* copy = hkl_string_new();

  if (hkl_string_compare(runes, copy) == false)
  {
    printf("The strings are not equal!\n");
    hkl_string_copy(copy, runes);
  }

  if (hkl_string_compare(runes, copy) == true)
  {
    printf("The strings are now equal!\n");
  }

  hkl_string_free(runes);
  runes = NULL;

  printf("%zu Norse runes: \"%s\"\n"
           "Occupying %zu bytes.\n",
           hkl_string_get_length(copy),
           hkl_string_get_utf8(copy),
           hkl_string_get_size(copy));

  hkl_string_free(copy);
  copy = NULL;

  HklString* hello = hkl_string_new_from_utf8("Hello");

  hkl_string_cat_utf8(hello, " World!");

  printf("%s\n", hkl_string_get_utf8(hello));

  hkl_string_free(hello);
  hello = NULL;
	
  return 0;
}
