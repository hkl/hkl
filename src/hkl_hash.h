#ifndef HKL_HASH_H
#define HKL_HASH_H

#include <stddef.h>
#include <stdbool.h>

#include "hkl_string.h"
#include "hkl_tree.h"

/**
@struct HklHash A hash table that just works.

@authors Jared Sealey, Scott LaVigne
@date 9/20/2012
*/
typedef struct HklHash
{
  
  struct HklHashElement* buckets;
  size_t length;
  size_t size;

} HklHash;

/**
Allocate a new HklHash to store data in.

@retval HklHash* the new hash table object.
*/
HklHash* hkl_hash_new();

/**
Insert or update a HklHash entry.

@param hash The HklHash to search.
@param key The string to match.
@param value A generic pointer.

@post The entered key is copied into an internal HklPair in the table.
      the key passed must still be managed by the caller.
      
@brief Values "stored" are not managed by the table. All entered data must still
       be freed as normal.
*/
void hkl_hash_insert(HklHash* hash, HklString* key, void* value);

/**
Locate an entry in a HklHash.

@param hash The HklHash to search.
@param key The string to match.
@retval HklPair* A pointer to the entry in the hash table.
*/
HklPair* hkl_hash_search(const HklHash* hash, HklString *key); 

/**
Remove a HklHash entry.

@param hash The HklHash to search.
@param key The string to match.
*/
void hkl_hash_remove(HklHash* hash, HklString *key);

/**
Clear all entries from a HklHash.

@param hash The HklHash to clear.
*/
void hkl_hash_clear(HklHash* hash);

/**
Free a HklHash and release any allocated resources it has.

@param hash The HklHash to free.
*/
void hkl_hash_free(HklHash* hash);

#endif // HKL_STRING_H
