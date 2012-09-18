#ifndef HKL_HASH_H
#define HKL_HASH_H
#include "hkl_string.h"
#include <stdbool.h>
#include <stddef.h>

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

HklHash* hkl_hash_new();

void hkl_hash_free(HklHash* const hash);

void hkl_hash_insert(HklHash* hash, HklString *key, void* value);

void* hkl_hash_find(HklHash* hash, HklString *key); 

void hkl_hash_delete(HklHash* hash, HklString *key);

#endif // HKL_STRING_H
