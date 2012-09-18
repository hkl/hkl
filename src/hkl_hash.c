#include "hkl_hash.h"
#include "hkl_alloc.h"

/*
{
  HklString *key;
  void *value;
  bool isRedBlackTree;
} HklHashElement;
*/
static size_t rot(size_t k, size_t r1)
{
  return (k<<r1) | (k>>(sizeof(size_t)*8 - r1));
}

static size_t Murmur3(const char *key, size_t len) {
  #define mmix3(h,k) { k *= m1; k = rot(k,r1); k *= m2; h *= 3; h ^= k; }
  const size_t seed = 0xBABABABA;
  const size_t m1 = 0x0acffe3d, m2 = 0x0e4ef5f3, m3 = 0xa729a897;
  const size_t r1 = 11, r2 = 18, r3 = 18;

  size_t h = len + seed, k = 0;

  const size_t *dwords = (const size_t *)key;
  while( len >= sizeof(size_t)) {
    k = *dwords++;
    mmix3(h,k);
    len -= sizeof(size_t);
  }

  const char *tail = (const char *)dwords;
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

HklHash* hkl_hash_new()
{
  HklHash *hash = hkl_alloc_object(HklHash);
  hash->maxSize = 16;
  hash->numKeys = 0;
  hash->elements = malloc(sizeof(HklHashElement)*hash->maxSize);
  return hash;
}

void hkl_hash_free(HklHash* const hash)
{
  free(hash->elements);
  free(hash);
}

void hkl_hash_insert(HklHash* hash, HklString *key, void* value)
{
  size_t index =  Murmur3( hkl_string_get_utf8(key), hkl_string_get_length(key)) % hash->maxSize;
  if(hash->elements[index].isRedBlackTree) 
  {
    // Insert into red black tree
  }
  else
  {
    hash->elements[index].value = value;
    hash->elements[index].key = hkl_string_new_from_string(key);
  }
}

void* hkl_hash_find(HklHash* hash, HklString *key)
{
  size_t index =  Murmur3(hkl_string_get_utf8(key), hkl_string_get_length(key)) % hash->maxSize;
  if(hash->elements[index].isRedBlackTree)
  {
    // find in red black tree
    return NULL;
  }
  else
  {
    return hash->elements[index].value;
  }
}

void hkl_hash_delete(HklHash* hash, HklString *key);
