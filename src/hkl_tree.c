#include <stdbool.h>
#include <string.h>

#include <assert.h>

#include "hkl_tree.h"
#include "hkl_alloc.h"

HklPair* hkl_pair_new()
{
  HklPair* pair = hkl_alloc_object(HklPair);

  pair->key = hkl_string_new();
  pair->value = NULL;

  return pair;
}

HklPair* hkl_pair_new_from_data(HklString* key, void* value)
{
  assert(key != NULL);

  HklPair* pair = hkl_pair_new();

  hkl_string_copy(pair->key, key);
  pair->value = value;

  return pair;
}

HklPair* hkl_pair_new_from_pair(HklPair* pair)
{
  assert(pair != NULL);

  return hkl_pair_new_from_data(pair->key, pair->value);
}

void hkl_pair_free(HklPair* pair)
{
  hkl_string_free(pair->key);
  hkl_free_object(pair);
}

typedef struct HklTreeNode
{
  HklPair* pair;
  struct HklTreeNode *left, *right;
  bool isred;

} HklTreeNode;

struct HklTree
{
  struct HklTreeNode* root;
};

static HklTreeNode* hkl_treenode_new(HklString* key, void* value)
{
  assert(key != NULL);

  HklTreeNode* node = hkl_alloc_object(HklTreeNode);

  // Duplicate the string to store in the node
  node->pair = hkl_pair_new_from_data(key, value);

  node->left = node->right = NULL;

  node->isred = true;

  return node;
}

static void hkl_treenode_free(HklTreeNode* node)
{
  assert(node != NULL);
  
  if (node->left != NULL)
    hkl_treenode_free(node->left);

  if (node->right != NULL)
    hkl_treenode_free(node->right);

  hkl_pair_free(node->pair);
  hkl_free_object(node);
}

static void hkl_treenode_colorflip(HklTreeNode* node)
{
  assert(node != NULL);

  node->isred = !node->isred;

  if (node->left != NULL)
    node->left->isred = !node->left->isred;

  if (node->right != NULL)
    node->right->isred = !node->right->isred;
}

static int hkl_treenode_isred(HklTreeNode* node)
{
  return ((node != NULL) && node->isred);
}

static HklTreeNode* hkl_treenode_rotleft(HklTreeNode* node)
{
  assert(node != NULL);
  assert(node->right != NULL);

  HklTreeNode* right = node->right;
  node->right = right->left;
  right->left = node;
  right->isred = node->isred;
  node->isred = true;
  return right;
}

static HklTreeNode* hkl_treenode_rotright(HklTreeNode* node)
{
  assert(node != NULL);
  assert(node->left != NULL);

  HklTreeNode* left = node->left;
  node->left = left->right;
  left->right = node;
  left->isred = node->isred;
  node->isred = true;
  return left;
}

static HklTreeNode* hkl_treenode_redleft(HklTreeNode* node)
{
  assert(node != NULL);

  hkl_treenode_colorflip(node);

  if (node->right != NULL && hkl_treenode_isred(node->right->left))
  {
    node->right = hkl_treenode_rotright(node->right);
    node = hkl_treenode_rotleft(node);
    hkl_treenode_colorflip(node);
  }

  return node;
}

static HklTreeNode* hkl_treenode_redright(HklTreeNode* node)
{
  assert(node != NULL);

  hkl_treenode_colorflip(node);

  if (node->left != NULL && hkl_treenode_isred(node->left->left))
  {
    node = hkl_treenode_rotright(node);
    hkl_treenode_colorflip(node);
  }

  return node;
}

static HklTreeNode*
hkl_treenode_insert(HklTreeNode* node, HklString* key, void* value)
{
  assert(key != NULL);

  if (node == NULL)
    return hkl_treenode_new(key, value);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->right))
    hkl_treenode_colorflip(node);

  int cmp = hkl_string_compare(node->pair->key, key);
  if (cmp == 0)     node->pair->value = value;
  else if (cmp < 0) node->left = hkl_treenode_insert(node->left, key, value);
  else              node->right = hkl_treenode_insert(node->right, key, value);

  if (hkl_treenode_isred(node->right) && !hkl_treenode_isred(node->left))
    node = hkl_treenode_rotleft(node);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->left->left))
    node = hkl_treenode_rotright(node);

  return node;
}

static HklTreeNode* hkl_treenode_fixup(HklTreeNode* node)
{
  assert(node != NULL);

  if (hkl_treenode_isred(node->right))
    node = hkl_treenode_rotleft(node);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->left->left))
    node = hkl_treenode_rotright(node);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->left))
    hkl_treenode_colorflip(node);
  
  return node;
}

static HklTreeNode* hkl_treenode_removemin(HklTreeNode* node)
{
  if (node->left == NULL)
  {
    hkl_treenode_free(node);    
    return NULL;
  }
 
  if (!hkl_treenode_isred(node->left) && !hkl_treenode_isred(node->left->left))
    node = hkl_treenode_redleft(node);

  node->left = hkl_treenode_removemin(node->left);

  return hkl_treenode_fixup(node);
}

static HklTreeNode* hkl_treenode_findmin(HklTreeNode* node)
{
  while (node->left != NULL)
    node = node->left;

  return node;
}

static HklTreeNode* hkl_treenode_remove(HklTreeNode* node, HklString* key)
{
  assert(node != NULL);
  assert(key != NULL);

  if (hkl_string_compare(key, node->pair->key) < 0)
  {
    if (node->left != NULL)
    {
      if (!hkl_treenode_isred(node->left) && hkl_treenode_isred(node->left->left))
        node = hkl_treenode_redleft(node);

      node->left = hkl_treenode_remove(node->left, key);
    }
  }
  else
  {
    if (hkl_treenode_isred(node->left))
      node = hkl_treenode_rotright(node);

    if (hkl_string_compare(key, node->pair->key) == 0 && (node->right == NULL))
    {
      hkl_treenode_free(node);
      return NULL;
    }

    if (node->right != NULL)
    {

      if (!hkl_treenode_isred(node->right) && !hkl_treenode_isred(node->right->left))
        node = hkl_treenode_redright(node);

      if (hkl_string_compare(key, node->pair->key) == 0)
      {
        node->pair->value = hkl_treenode_findmin(node->right)->pair->value;
        node->right = hkl_treenode_removemin(node->right);
      }
      else
      {
        node->right = hkl_treenode_remove(node->right, key);
      }
    }
  }

  return hkl_treenode_fixup(node);
}

HklTree* hkl_tree_new()
{
  HklTree* tree = hkl_alloc_object(HklTree);

  tree->root = NULL;

  return tree;
}

HklPair* hkl_tree_search(HklTree* tree, HklString* key)
{
  assert(tree != NULL);
  assert(key != NULL);

  HklTreeNode* node = tree->root;
  int cmp = 0;

  while (node != NULL)
  {
    cmp = hkl_string_compare(node->pair->key, key);

    if (cmp == 0)
    {
      return node->pair;
    }
    else if (cmp < 0)
    {
      node = node->left;
    }
    else
    {
      node = node->right;
    }
  }

  return NULL;
}

void hkl_tree_insert(HklTree* tree, HklString* key, void* value)
{
  assert(tree != NULL);
  assert(key != NULL);

  tree->root = hkl_treenode_insert(tree->root, key, value);
  tree->root->isred = false;
}

void hkl_tree_remove(HklTree* tree, HklString* key)
{
  assert(tree != NULL);
  assert(key != NULL);

  if (tree->root != NULL)
  {
    tree->root = hkl_treenode_remove(tree->root, key);

    if (tree->root != NULL)
    {
      tree->root->isred = false;
    }
  }
}

void hkl_tree_clear(HklTree* tree)
{
  assert(tree != NULL);

  if (tree->root != NULL)
    hkl_treenode_free(tree->root);
  
  tree->root = NULL;
}

void hkl_tree_free(HklTree* tree)
{
  hkl_tree_clear(tree);
  hkl_free_object(tree);
}

static void hkl_treenode_traverse(HklTreeNode* node,
  void(*fn)(HklPair*, void*), void* data)
{
  assert(node != NULL);

  if (node->left != NULL)
    hkl_treenode_traverse(node->left, fn, data);

  fn(node->pair, data);

  if (node->right != NULL)
      hkl_treenode_traverse(node->right, fn, data);
}

void hkl_tree_traverse(HklTree* tree, void(*fn)(HklPair*, void*), void* data)
{
  assert(tree != NULL);

  if (tree->root != NULL)
    hkl_treenode_traverse(tree->root, fn, data);
}

// This is the form for moved pairs
// This does not make a copy of a pair
static HklTreeNode* hkl_treenode_new_from_pair(HklPair* pair)
{
  assert(pair != NULL);

  HklTreeNode* node = hkl_alloc_object(HklTreeNode);

  node->pair = pair;

  node->left = node->right = NULL;

  node->isred = true;

  return node;
}

static HklTreeNode* hkl_treenode_move_pair(HklTreeNode* node, HklPair* pair)
{
  assert(pair != NULL);

  if (node == NULL)
    return hkl_treenode_new_from_pair(pair);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->right))
    hkl_treenode_colorflip(node);

  int cmp = hkl_string_compare(node->pair->key, pair->key);
  if (cmp == 0)
  { 
    // This shouldnt happen since we only move into empty trees    
    assert(false);
    node->pair = pair;
  }
  else if (cmp < 0) node->left = hkl_treenode_move_pair(node->left, pair);
  else              node->right = hkl_treenode_move_pair(node->right, pair);

  if (hkl_treenode_isred(node->right) && !hkl_treenode_isred(node->left))
    node = hkl_treenode_rotleft(node);

  if (hkl_treenode_isred(node->left) && hkl_treenode_isred(node->left->left))
    node = hkl_treenode_rotright(node);

  return node;
}

void hkl_tree_move_pair(HklTree* tree, HklPair* pair)
{
  assert(tree != NULL);
  assert(pair != NULL);

  tree->root = hkl_treenode_move_pair(tree->root, pair);
  tree->root->isred = false; 
}
