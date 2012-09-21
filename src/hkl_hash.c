#include <assert.h>

#include "hkl_hash.h"
#include "hkl_alloc.h"

static size_t Murmur3(const char* key, size_t len) {
  
  #define mmix3(h,k) { k *= m1; k = (k<<r1)|(k>>(sizeof(size_t)*8-r1)); \
  k *= m2; h *= 3; h ^= k; }

  const size_t seed = 0xDEADBEEF;
  const size_t m1 = 0x0acffe3d, m2 = 0x0e4ef5f3, m3 = 0xa729a897;
  const size_t r1 = 11, r2 = 18, r3 = 18;

  size_t h = len + seed, k = 0;

  const size_t* dwords = (const size_t*) key;
  while(len >= sizeof(size_t)) {
    k = *dwords++;
    mmix3(h,k);
    len -= sizeof(size_t);
  }

  const char* tail = (const char*) dwords;
  switch( len ) {
    case 3: k ^= tail[2] << 16;
    case 2: k ^= tail[1] << 8;
    case 1: k ^= tail[0];
      mmix3(h,k);
  };

  h ^= h >> r2;
  h *= m3;
  h ^= h >> r3;
  return h;
}

static void hkl_hash_double(HklHash* hash)
{
  assert(hash != NULL);

  // Double tree
}

typedef struct HklHashElement
{
  bool is_tree;
  void* data;

} HklHashElement;

HklHash* hkl_hash_new()
{
  HklHash* hash = hkl_alloc_object(HklHash);

  hash->size = 16;
  hash->length = 0;

  // Allocate space for each bucket
  hash->buckets = (HklHashElement*)malloc(sizeof(HklHashElement)*hash->size);

  // Initialize the buckets
  size_t i;
  for (i = 0; i < hash->size; ++i)
  {
    hash->buckets[i].is_tree = false;
    hash->buckets[i].data = NULL;
  }

  return hash;
}

void hkl_hash_free(HklHash* hash)
{
  assert(hash != NULL);

  hkl_hash_clear(hash);
  hkl_free_object(hash);
}

void hkl_hash_insert(HklHash* hash, HklString* key, void* value)
{
  assert(hash != NULL);
  assert(key != NULL);

  size_t index = Murmur3(hkl_string_get_utf8(key), hkl_string_get_length(key))
                 % hash->size;

  HklHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  if (element->data != NULL)
  {
    if (element->is_tree)
    {
      hkl_tree_insert((HklTree*) element->data, key, value);
    }
    else
    {
      // First Collision
      HklPair* pair = element->data;

      element->data = hkl_tree_new();
      hkl_tree_insert((HklTree*) element->data, pair->key, pair->value);

      // Free the uneeded pair
      hkl_pair_free(pair);
      hkl_tree_insert((HklTree*) element->data, key, value);

      // Mark the element as a tree
      element->is_tree = true;
    }
  }
  else
  {
    // Nothing exists here. Make a pair
    element->data = hkl_pair_new_from_data(key, value);

    // The number of entries of the table increases
    ++hash->length;

    /*// If the hash table is 75% full
    if (hash->length >= 0.75*hash->size)
    {
      // Grow the table
      hkl_hash_double(hash);
    }*/
  }
}

HklPair* hkl_hash_search(const HklHash* hash, HklString *key)
{
  assert(hash != NULL);
  assert(key != NULL);

  size_t index = Murmur3(hkl_string_get_utf8(key), hkl_string_get_length(key))
                 % hash->size;

  HklHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  if (element->data != NULL)
  {

    if (element->is_tree)
    {
      return hkl_tree_search((HklTree*) element->data, key);
    }
    else
    {
      return (HklPair*) element->data;
    }
  }
  else
  {
    // No such element, do nothing
    return NULL;
  }
}

void hkl_hash_remove(HklHash* hash, HklString *key)
{
  assert(hash != NULL);
  assert(key != NULL);

  size_t index = Murmur3(hkl_string_get_utf8(key), hkl_string_get_length(key))
                 % hash->size;

  HklHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  if (element->data != NULL)
  {

    if (element->is_tree)
    {
      hkl_tree_remove((HklTree*) element->data, key);
    }
    else
    {
      hkl_pair_free((HklPair*) element->data);
      --hash->length;
    }
  }
}

void hkl_hash_clear(HklHash* hash) {

  assert(hash != NULL);

  size_t i;

  HklHashElement* element = NULL;

  for (i = 0; i < hash->size; ++i)
  {
    element = &hash->buckets[i];

    if (element->is_tree)
      hkl_tree_free((HklTree*) element->data);

    else if (element->data)
      hkl_pair_free((HklPair*) element->data);

    element->is_tree = false;
    element->data = NULL;
  }

  hash->length = 0;
}
