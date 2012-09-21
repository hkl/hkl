#ifndef HKL_TREE_H
#define HKL_TREE_H

#include "hkl_string.h"

/**
@struct HklTree A simple Associative array implementation based off of the paper
"Left-leaning Red-Black Trees" by Robert Sedgewick

@authors Scott LaVigne
@date 9/19/2012
*/
typedef struct HklTree HklTree;

/**
@struct HklPair a key-value pair container for hkl associative arrays.
*/
typedef struct HklPair {

  HklString* key;
  void* value;

} HklPair;

/**
Allocate a new Key-Value pair.

@retval HklPair* A new HklPair.
@brief Allocates a new empty HklPair.
*/
HklPair* hkl_pair_new();

/**
Allocate a new Key-Value pair.

@param key A HklString.
@param value A generic pointer.
@retval HklPair* A new HklPair.
@brief Allocates a new HklPair.
*/
HklPair* hkl_pair_new_from_data(HklString* key, void* value);

/**
Allocate a new Key-Value pair as a copy of an existing pair.

@param pair A HklPair to copy.
@retval HklPair* A new HklPair.
@brief Allocates a new empty HklPair.
*/
HklPair* hkl_pair_new_from_pair(HklPair* pair);

/**
Free a HklPair and releases any allocated resources it has.

@param pair The HklPair to free.
*/
void hkl_pair_free(HklPair* pair);

/**
Allocate a new HklTree to store data in.

@retval HklTree* the new tree object.
*/
HklTree* hkl_tree_new();

/**
Locate an entry in a HklTree.

@param tree The HklTree to search.
@param key The string to match.
@retval HklPair* A pointer to the entry in the tree.
*/
HklPair* hkl_tree_search(HklTree* tree, HklString* key);

/**
Insert or update a HklTree entry.

@param tree The HklTree to search.
@param key The string to match.
@param value A generic pointer.

@post The entered key is copied into an internal HklPair in the tree.
      the key passed must still be managed by the caller.

@brief Values "stored" are not managed by the tree. All entered data must still
       be freed as normal.
*/
void hkl_tree_insert(HklTree* tree, HklString* key, void* value);

/**
Remove a HklTree entry.

@param tree The HklTree to search.
@param key The string to match.
*/
void hkl_tree_remove(HklTree* tree, HklString* key);

/**
Clear all entries from a HklTree.

@param tree The HklTree to clear.
*/
void hkl_tree_clear(HklTree* tree);

/**
Free a HklString and release any allocated resources it has.

@param tree The HklTree to free.
*/
void hkl_tree_free(HklTree* tree);

void hkl_tree_traverse(HklTree* tree, void(*fn)(HklPair*, void*), void* data);

#endif // HKL_TREE_H