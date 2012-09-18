#ifndef HKL_HASH_H
#define HKL_HASH_H
#include "hkl_string.h"
#include <stdbool.h>
#include <stddef.h>

/**
@struct HklHash: a hash table implementation using MurmurHash3 and red black trees for collision handling.

@authors Scott Lavigne, Jared Sealey
@data 9/16/2012
*/

typedef struct HklHashElement
{
  HklString *key;
  void *value;
  bool isRedBlackTree;
} HklHashElement;

typedef struct HklHash
{
  HklHashElement *elements;
  size_t numKeys;
  size_t maxSize;
} HklHash;

/**
Allocate a new HklHash.

@retval HklHash* A new HklHash.
@brief Allocates a new empty HklHash.
*/
HklHash* hkl_hash_new();

/**
Insert a new HklHashElement into a HklHash

@param hash The hash table to insert the element into.
@param key The key of the HklHashElement
@param value The value of the HklHashElement
*/
void hkl_hash_insert(HklHash* hash, HklString *key, void* value);

/**
Find an HklHashElement in an HklHash and return its value.

@param hash The hash table to search
@param key The key to search with
@retval void* The value if found, NULL otherwise
*/
void* hkl_hash_find(HklHash* hash, HklString *key); 

/**
Delete an HklHashElement in an HklHash

@param hash The hash table containing the element to delete
@param key The key of the element to delete
*/
void hkl_hash_delete(HklHash* hash, HklString *key);

/**
Free an HklHash and all of its allocated resources

@param hash The HklHash to free
*/
void hkl_hash_free(HklHash* const hash);

#endif // HKL_STRING_H
