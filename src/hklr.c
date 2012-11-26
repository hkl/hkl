#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "hkl_alloc.h"

#include "hklr.h"

void hklr_init()
{
  // The first and last gc_root are dummy objects
  HKLR.gc_roots = hklr_object_new(HKL_TYPE_INT, HKL_FLAG_NONE, 80075);
  HKLR.gc_tail = hklr_object_new(HKL_TYPE_INT, HKL_FLAG_NONE, 1457);
  // Dont count them in the object count
  HKLR.gc_created-=2;

  HKLR.gc_roots->next = HKLR.gc_tail;
  HKLR.gc_tail->prev = HKLR.gc_roots;

  HKLR.gc_to_free = hkl_list_new();

  HKLR.gc_runs = 0;
  HKLR.gc_freed = 0;
  HKLR.gc_rootsize = 0;

  HKLR.globals = hkl_hash_new();
  HKLR.scopes = hkl_list_new();
  HKLR.scope_level = 0;

  hklr_scope_push();
}

static bool hklr_gc_dec_hash(HklPair* pair, void* data)
{
  hklr_gc_dec((HklrObject*) pair->value);

  return false;
}

void hklr_shutdown()
{
  hklr_scope_pop();
  hkl_list_free(HKLR.scopes);

  // free globals
  hkl_hash_traverse(HKLR.globals, hklr_gc_dec_hash, NULL);
  hkl_hash_free(HKLR.globals);

  // collect garbage
  hklr_gc_collect();

  hkl_list_free(HKLR.gc_to_free);

  hklr_object_free(HKLR.gc_roots);
  hklr_object_free(HKLR.gc_tail);
}

void hklr_scope_push()
{
  HklScope* scope = hkl_alloc_object(HklScope);

  scope->locals = hkl_hash_new();
  scope->upvals = hkl_hash_new();

  hkl_list_push_back(HKLR.scopes, scope);
}

void hklr_scope_pop()
{
  HklScope* scope = hkl_list_pop_back(HKLR.scopes);

  // decrement all the locals
  hkl_hash_traverse(scope->locals, hklr_gc_dec_hash, NULL);

  // dont dec upvals as they arent in our scope
  // hkl_hash_traverse(scope->upvals, hklr_gc_dec_hash, NULL);
  hkl_hash_free(scope->locals);
  hkl_hash_free(scope->upvals);

  hkl_free_object(scope);
}

void hklr_local_insert(HklString* key, HklrObject* value)
{
  hkl_hash_insert(((HklScope*) HKLR.scopes->tail->data)->locals, key, value);
}

void hklr_upval_insert(HklString* key, HklrObject* value)
{
  hkl_hash_insert(((HklScope*) HKLR.scopes->tail->data)->upvals, key, value);
}

void hklr_global_insert(HklString* key, HklrObject* value)
{
  hkl_hash_insert(HKLR.globals, key, value);
}

HklrObject* hklr_search(HklString* key)
{
  HklrObject* object = hklr_exists(key);

  if (object == NULL)
  {
    // Didn't find it, make a nil object
    object = hklr_object_new(HKL_TYPE_NIL, HKL_FLAG_NONE);
    hklr_local_insert(key, object);
  }

  return object;
}

HklrObject* hklr_exists(HklString* key)
{
  assert(key != NULL);

  HklScope* scope = ((HklScope*) HKLR.scopes->tail->data);
  HklPair* pair = NULL;

  // check your scope first
  pair = hkl_hash_search(scope->locals, key);
  if (pair) return pair->value; 

  // now try upvals
  pair = hkl_hash_search(scope->upvals, key);
  if (pair) return pair->value;

  // Now try scopes above

  HklListNode* node = HKLR.scopes->tail;
  node = node->last;
  while(node != NULL)
  {
    scope = ((HklScope*) node->data);
    // check locals first then upvals
    pair = hkl_hash_search(scope->locals, key);
    if (!pair) pair = hkl_hash_search(scope->upvals, key);    

    // if you find the object 
    // make it an upval to my scope
    if (pair)
    {
      hklr_upval_insert(key, pair->value);
      return pair->value;
    }

    node = node->last;
  }

  // Finally try global scope
  pair = hkl_hash_search(HKLR.globals, key);
  if (pair)
  {
    hklr_upval_insert(key, pair->value);
    return pair->value;
  }

  return NULL;
}

void hklr_gc_inc(HklrObject* object)
{
  assert(object != NULL);

  object->rc++;
  object->color = HKL_COLOR_BLACK;
}

static void hklr_gc_possible_root(HklrObject* object)
{ 
  if (object->color != HKL_COLOR_PURPLE)
  {
    object->color = HKL_COLOR_PURPLE;

    if (!object->is_buffered)
    {
      object->is_buffered = true;

      // Append object to roots buffer
      object->prev = HKLR.gc_tail->prev;
      HKLR.gc_tail->prev = object;
      object->prev->next = object;
      object->next = HKLR.gc_tail;

      HKLR.gc_rootsize++;
    }
  }
}

static void hklr_gc_release(HklrObject* object)
{
  switch (object->type)
  {
    // If the object is a hash table
    case HKL_TYPE_HASH:
      // Traverse the hash decrementing every child
      hkl_hash_traverse(object->as.hash, hklr_gc_dec_hash, NULL);
      break;

    case HKL_TYPE_ARRAY:

      break;

    // If the object is a reference
    case HKL_TYPE_REF:
      hklr_gc_dec(object->as.object);
      break; 

    default:
    break;
  }

  object->color = HKL_COLOR_BLACK;

  if (!object->is_buffered)
  {
    HKLR.gc_freed++;
    hklr_object_free(object);
  }
}

void hklr_gc_dec(HklrObject* object)
{
  assert(object != NULL);

  object->rc--;
  if (object->rc == 0)
  {
    hklr_gc_release(object);
  }
  else
  {
    hklr_gc_possible_root(object);
  }
}

static void hklr_gc_scanblack(HklrObject* object);
static bool hklr_gc_scanblack_hash(HklPair* pair, void* data)
{
  HklrObject* object = (HklrObject*) pair->value;
  object->rc++;

  if (object->color != HKL_COLOR_BLACK)
  {
    hklr_gc_scanblack(object);
  }

  return false;
}

static void hklr_gc_scanblack(HklrObject* object)
{
  object->color = HKL_COLOR_BLACK;

  switch (object->type)
  {
    // If the object is a hash table
    case HKL_TYPE_HASH:
      // Traverse the hash scanning every child
      hkl_hash_traverse(object->as.hash, hklr_gc_scanblack_hash, NULL);
      break;

    case HKL_TYPE_ARRAY:
      // Traverse the deque scanning every child
      break;

    // If the object is a reference
    case HKL_TYPE_REF:
      hklr_gc_scanblack(object->as.object);
      break;

    default:
      assert(false);
    break;
  }
}

// Foward declaration
static void hklr_gc_markgray(HklrObject* object);
static bool hklr_gc_markgray_hash(HklPair* pair, void* data)
{
  HklrObject* object = pair->value;
  object->rc--;
  hklr_gc_markgray(object);

  return false;
}

static void hklr_gc_markgray(HklrObject* object)
{
  if (object->color != HKL_COLOR_GRAY)
  {
    object->color = HKL_COLOR_GRAY;

    switch (object->type)
    {
      // If the object is a hash table
      case HKL_TYPE_HASH:
        // Traverse the hash marking every child
        hkl_hash_traverse(object->as.hash, hklr_gc_markgray_hash, NULL);
        break;

      case HKL_TYPE_ARRAY:
        break;

      // If the object is a reference
      case HKL_TYPE_REF:

        object->as.object->rc--;
        hklr_gc_markgray(object->as.object);
        break;

      default:
        assert(false);
      break;
    }
  }
}

static void hklr_gc_markroots()
{
  // For all possible roots
  HklrObject* s = HKLR.gc_roots->next;
  for ( ; s != HKLR.gc_tail; s = s->next)
  {
    if (s->color == HKL_COLOR_PURPLE && s->rc > 0)
    {
      hklr_gc_markgray(s);
    }
    else
    {
      s->is_buffered = false;

      // Remove node from possible roots
      s->prev->next = s->next;
      s->next->prev = s->prev;

      HKLR.gc_rootsize--;

      if (s->color == HKL_COLOR_BLACK && s->rc == 0)
      {
        //HKLR.gc_freed++;
        //hklr_object_free(s);
        // queue the object for collection
        hkl_list_push_back(HKLR.gc_to_free, s);
      }
    }
  }
}

// Foward declaration
static void hklr_gc_scan(HklrObject* object);
static bool hklr_gc_scan_hash(HklPair* pair, void* data)
{
  hklr_gc_scan((HklrObject*) pair->value);

  return false;
}

static void hklr_gc_scan(HklrObject* object)
{
  if (object->color == HKL_COLOR_GRAY)
  {
    if (object->rc > 0)
    {
      hklr_gc_scanblack(object);
    }
    else
    {
      object->color = HKL_COLOR_WHITE;

      switch (object->type)
      {
        // If the object is a hash table
        case HKL_TYPE_HASH:
          // Traverse the hash scanning every child
          hkl_hash_traverse(object->as.hash, hklr_gc_scan_hash, NULL);
          break;

        // If the object is a reference
        case HKL_TYPE_REF:
          hklr_gc_scan(object->as.object);
          break;

        default:
          assert(false);
        break;
      }
    }
  }
}

static void hklr_gc_scanroots()
{
  // For all possible roots
  HklrObject* s = HKLR.gc_roots->next;
  for ( ; s != HKLR.gc_tail; s = s->next)
  {
    hklr_gc_scan(s);
  }
}

static void hklr_gc_collectwhite(HklrObject* object);
static bool hklr_gc_collectwhite_hash(HklPair* pair, void* data)
{
  hklr_gc_collectwhite((HklrObject*) pair->value);

  return false;
}

static void hklr_gc_collectwhite(HklrObject* object)
{
  assert(object != NULL);

  if (object->color == HKL_COLOR_WHITE && !object->is_buffered)
  {
    object->color = HKL_COLOR_BLACK;

    switch (object->type)
    {
      // If the object is a hash table
      case HKL_TYPE_HASH:
        // Traverse the hash scanning every child
        hkl_hash_traverse(object->as.hash, hklr_gc_collectwhite_hash, NULL);
        break;

      // If the object is a reference
      case HKL_TYPE_REF:
        hklr_gc_collectwhite(object->as.object);
        break;

      default:
        assert(false);
      break;
    }

    hkl_list_push_back(HKLR.gc_to_free, object);
  }
}

static void hklr_gc_collectroots()
{
  // For all possible roots
  HklrObject* s = HKLR.gc_roots->next;
  for ( ; s != HKLR.gc_tail; s = s->next)
  {
    // Remove node from possible roots
    s->prev->next = s->next;
    s->next->prev = s->prev;

    HKLR.gc_rootsize--;

    s->is_buffered = false;
    hklr_gc_collectwhite(s);
  }

  // Free the queued roots
  while ((s = (HklrObject*) hkl_list_pop_front(HKLR.gc_to_free)))
  {
    HKLR.gc_freed++;

    hklr_object_free(s);
  }
}

void hklr_gc_collect()
{
  hklr_gc_markroots();
  hklr_gc_scanroots();
  hklr_gc_collectroots();

  HKLR.gc_runs++;
}
