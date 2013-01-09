#ifndef HKL_TREE_H
#define HKL_TREE_H

#include <stddef.h>

#include "hkl_string.h"

/**
@struct HklTree A simple Associative array implementation based off of the paper
"Left-leaning Red-Black Trees" by Robert Sedgewick

@authors Scott LaVigne, Chris Knadler
@date 9/19/2012
*/

/**
@struct HklPair a key-value pair container for hkl associative arrays.
*/
typedef struct HklPair {
  HklString* key;
  void* value;
} HklPair;

/**
@struct HklTreeNode a node in the HklTree.
*/
typedef struct HklTreeNode
{
  HklPair* pair;
  struct HklTreeNode* left;
  struct HklTreeNode* right;
  bool isred;
} HklTreeNode;

/**
@struct HklTree the tree object.
*/

typedef struct HklTree
{
  HklTreeNode* root;
  size_t size;
} HklTree;

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
Allocate a new Key-Value pair.

@param key A utf8_string.
@param value A generic pointer.
@retval HklPair* A new HklPair.
@brief Allocates a new HklPair.
*/
HklPair* hkl_pair_new_from_utf8(const char* key, void* value);

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

void hkl_tree_traverse(HklTree* tree, bool(*fn)(HklPair*, void*), void* data);

/**
Move a pair into a HklTree.

@param tree The HklTree.
@param pair The pair to move into the tree.

@brief WARNING: Moved pairs should only be owned by one tree
       the recieving tree gets owership of the pair and will free
       it automatically.
*/
void hkl_tree_move_pair(HklTree* tree, HklPair* pair);

#endif // HKL_TREE_H
