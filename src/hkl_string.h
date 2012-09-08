#ifndef HKL_STRING_H
#define HKL_STRING_H

#include <stdint.h>
#include <stdbool.h>

/**
@struct HklString A utf8-based string implementation.

Since utf-8 internally can use anywhere between 1 and 4 bytes per character
HklString acts as a wrapper for utf-8 data.

@authors Scott LaVigne
@date 9/7/2012
*/
typedef struct HklString
{
  char* utf8_data; /* Private */
  size_t size;        /* Private */
  size_t length;     /* Private */

} HklString;

/**
Allocate a new HklString.

@retval HklString* A new HklString.
@brief Allocates a new empty HklString.
*/
HklString* hkl_string_new();

/**
Allocate a copy of another HklString.

@param string The string to make a copy of.
@retval HklString* A newly allocated copy of string.
*/
HklString* hkl_string_new_from_string(const HklString* string);

/**
Allocate a new HklString using utf8 data.

@param utf8str The null-terminated utf8 data to copy into the HklString.
@retval HklString* A new HklString.
@brief Allocates a new HklString, with an internal copy of a utf8 string.
*/
HklString* hkl_string_new_from_utf8(const char* utf8_data);

/**
Sets the utf8 data inside a HklString.

@param string The HklString to modify.
@param utf8str The null-terminated utf8 data to copy into the HklString.
@brief Modifies string with a copy of a utf8 string.
*/
void hkl_string_set_utf8(HklString* string, const char* utf8_data);

/**
Concatinate a HklString.

@param string The HklString to add data to.
@param src The HklString to append.
@brief The string is modified in-place only reallocating internal data if needed.
*/
void hkl_string_cat(HklString* string, const HklString* src);

/**
Concatinate a HklString with utf8 data.

@param string The HklString to add data to.
@param utf8_data The null-terminated utf8 string to append.
@brief The string is modified in-place only reallocating internal data if needed.
*/
void hkl_string_cat_utf8(HklString* string, const char* utf8_data);

/**
Turns a HklString into an exact copy of another.

@param string The HklString to modify.
@param src The HklString to copy.
*/
void hkl_string_copy(HklString* string, const HklString* src);

/**
Compares two HklStrings.

@param string1 A HklString object.
@param string2 A second HklString object to compare with.
@retval bool True if the strings are equal, false otherwise.
*/
bool hkl_string_compare(const HklString* string1, const HklString* string2);

/**
Compares a HklString with utf8 data.

@param string A HklString object.
@param utf8_data A null-terminated utf8 string to compare.
@retval bool True if the strings are equal, false otherwise.
*/
//bool hkl_string_compare_utf8(HklString* string, const char* utf8_data);

/**
Erase the contents of a HklString

@post The internal utf8 data is freed.

@param string The HklString to clear.
*/
void hkl_string_clear(HklString* string);

/**
Get the raw utf8_data within a HklString

@param string The HklString object.
@retval uint8_t* A pointer to the utf8 data within the string
*/
const char* hkl_string_get_utf8(HklString* string);

/**
Get the number of utf8 bytes in a HklString.
This does not include the null byte.

@param string The HklString object.
@retval size_t The number of bytes in the string.
*/
size_t hkl_string_get_size(const HklString* string);

/**
Get the number of characters in the HklString.
This does not include the null character.

@param string The HklString object.
@retval size_t The number of characters in the string.
*/
size_t hkl_string_get_length(const HklString* string);

/**
Free a HklString and releases any allocated resources it has.

@param string The HklString to free.
*/
void hkl_string_free(HklString* string);

#endif // HKL_STRING_H
