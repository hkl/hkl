#include <assert.h>
#include <string.h>

#include "hkl_string.h"
#include "hkl_alloc.h"

static char conversion_buffer[16];

// Get length of utf8-encoded string
static size_t utf8_length(const char* utf8_data) {
   size_t i = 0, j = 0;

   while (utf8_data[i]) {

     if ((utf8_data[i] & 0xC0) != 0x80) j++;
     i++;
   }
   return j;
}

HklString* hkl_string_new()
{
  HklString* const string = hkl_alloc_object(HklString);
  assert(string != NULL);

  // allocate a null character
  string->utf8_data = malloc(1);
  string->utf8_data[0] = '\0';
  string->size = 1;
  string->length = 0;
  string->hash = 0;

  return string;
}

HklString* hkl_string_new_from_string(const HklString* string)
{
  assert(string != NULL);

  HklString* copy = hkl_string_new();
  hkl_string_copy(copy, string);

  return copy;
}

HklString* hkl_string_new_from_integer(int integer)
{
  // create a buffer to store the integer
  HklString* string = hkl_string_new();

  string->size = sprintf(conversion_buffer, "%d", integer) + 1;
  string->utf8_data = realloc(string->utf8_data, string->size);
  memcpy(string->utf8_data, conversion_buffer, string->size);
  string->length = utf8_length(string->utf8_data);
  
  return string;
}

HklString* hkl_string_new_from_real(double real)
{
  // create a buffer to store the integer
  HklString* string = hkl_string_new();

  string->size = sprintf(conversion_buffer, "%lf", real) + 1;
  string->utf8_data = realloc(string->utf8_data, string->size);
  memcpy(string->utf8_data, conversion_buffer, string->size);
  string->length = utf8_length(string->utf8_data);

  return string;
}

HklString* hkl_string_new_from_stream(FILE* stream)
{
  // initialize an 8 character input buffer
  HklString* string = hkl_string_new_from_utf8("       ");
  size_t input = 0;
  int c;

  while (true)
  {
    c = fgetc(stdin);

    if (c == EOF || c == '\n')
      break;

    // if we are about to overflow
    if (input == string->size)
    {
      // double the buffer
      string->size *= 2;
      string->utf8_data = realloc(string->utf8_data, string->size);
    }

    string->utf8_data[input] = c;
    input++;
  }

  // write a null character
  string->utf8_data[input] = '\0';

  string->length = utf8_length(string->utf8_data);

  return string;
}

HklString* hkl_string_new_from_utf8(const char* utf8_data)
{
  assert(utf8_data != NULL);

  HklString* string = hkl_string_new();
  hkl_string_set_utf8(string, utf8_data);

  return string;
} 

void hkl_string_set_utf8(HklString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(utf8_data != NULL);

  size_t size = strlen(utf8_data);
  
  // The input string must be null-terminated
  assert(utf8_data[size] == '\0');

  // Resize the string to accomidate new data
  if (string->size < size + 1)
    string->utf8_data = realloc(string->utf8_data, size + 1);

  // This used to only copy size, but that caused a horrible memory leak
  string->size = size + 1;
  string->length = utf8_length(utf8_data);

  memcpy(string->utf8_data, utf8_data, size + 1);

  // The string should have utf8_data
  assert(string->utf8_data != NULL);
  // The string must be NULL terminated by now
  assert(string->utf8_data[size] == '\0');

  // The hash is invalid
  string->hash = 0;
}

void hkl_string_cat(HklString* string, const HklString* src)
{
  assert(string != NULL);
  assert(src != NULL);

  // For now this works
  // Later this can be sped up by not relying on this function.
  hkl_string_cat_utf8(string, src->utf8_data);

  // The hash is invalid
  string->hash = 0;
}

void hkl_string_cat_utf8(HklString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(string->utf8_data != NULL);
  assert(utf8_data != NULL);

  size_t size = strlen(utf8_data);

  // The input string must be null-terminated
  assert(utf8_data[size] == '\0');

  // Resize the string to accomidate new data
  if (string->size < string->size + size + 1)
    string->utf8_data = realloc(string->utf8_data, string->size + size + 1);

  string->length += utf8_length(utf8_data);

  memcpy(string->utf8_data + string->size - 1, utf8_data, size + 1);

  string->size = string->size + size;

  // The string should have utf8_data
  assert(string->utf8_data != NULL);
  // The string must be NULL terminated by now
  assert(string->utf8_data[string->size - 1] == '\0');

  // The hash is invalid
  string->hash = 0;
}
  
void hkl_string_copy(HklString* string, const HklString* src)
{
  assert(string != NULL);
  assert(src != NULL);

  hkl_string_set_utf8(string, src->utf8_data);

  // Copy the hash from the src string
  // >_<'
  string->hash = src->hash;
}

int hkl_string_compare(const HklString* string1, const HklString* string2)
{
  assert(string1 != NULL);
  assert(string2 != NULL);

  // Since we are doing a strncmp, we need to make sure we pick the larger
  // of the two strings as the size
  size_t size = (string1->length > string2->length)?
    string1->length : string2->length;

  return strncmp(string1->utf8_data, string2->utf8_data, size);
}

int hkl_string_compare_utf8(HklString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(utf8_data != NULL);

  // Since we are doing a strncmp, we need to make sure we pick the larger
  // of the two strings as the size
  size_t ulen = utf8_length(utf8_data);
  size_t size = (string->length > ulen)?
    string->length : ulen;

  return strncmp(string->utf8_data, utf8_data, size);
}

void hkl_string_clear(HklString* string)
{
  assert(string != NULL);

  free(string->utf8_data);

  string->utf8_data = NULL;
  string->size = 0;
  string->length = 0;

  // The hash is invalid
  string->hash = 0;
}

const char* hkl_string_get_utf8(HklString* string)
{
  assert(string != NULL);

  return string->utf8_data;
}

size_t hkl_string_get_size(const HklString* string)
{
  assert(string != NULL);

  return string->size;
}

size_t hkl_string_get_length(const HklString* string)
{
  assert(string != NULL);

  return string->length;
}

void hkl_string_free(HklString* string)
{
  assert(string != NULL);

  hkl_string_clear(string);
  hkl_free_object(string);
}
