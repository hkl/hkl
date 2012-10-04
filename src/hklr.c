#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

#include "hklr.h"

void hklr_init()
{
  // The first and last gc_root are dummy objects
  HKLR.gc_roots = hklr_object_new(HKL_TYPE_INT, HKL_FLAG_NONE, NULL);
  HKLR.gc_tail = hklr_object_new(HKL_TYPE_INT, HKL_FLAG_NONE, NULL);
  // Dont count them in the object count
  HKLR.gc_created-=2;

  HKLR.gc_roots->next = HKLR.gc_tail;
  HKLR.gc_tail->prev = HKLR.gc_roots;

  HKLR.gc_runs = 0;
  HKLR.gc_freed = 0;
  HKLR.gc_rootsize = 0;
}

void hklr_shutdown()
{
  hklr_object_free(HKLR.gc_roots);
  hklr_object_free(HKLR.gc_tail);
}

void hklr_gc_inc(HklObject* object)
{
  object->rc++;
  object->color = HKL_COLOR_BLACK;
}

static void hklr_gc_possible_root(HklObject* object)
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

static void hklr_gc_dec_hash(HklPair* pair, void* data)
{
  hklr_gc_dec((HklObject*) pair->value);
}

static void hklr_gc_release(HklObject* object)
{

  switch (object->type)
  {
    // If the object is a hash table
    case HKL_TYPE_HASH:
      // Traverse the hash decrementing every child
      hkl_hash_traverse(object->as.hash, hklr_gc_dec_hash, NULL);
      break;

    // If the object is a reference
    case HKL_TYPE_REF:
      hklr_gc_dec(object->as.ref);
      break; 

    default: break;
  }

  object->color = HKL_COLOR_BLACK;

  if (!object->is_buffered)
  {
    HKLR.gc_freed++;
    hklr_object_free(object);
  }
}

void hklr_gc_dec(HklObject* object)
{
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

static void hklr_gc_scanblack(HklObject* object);
static void hklr_gc_scanblack_hash(HklPair* pair, void* data)
{
  HklObject* object = (HklObject*) pair->value;
  object->rc++;

  if (object->color != HKL_COLOR_BLACK)
  {
    hklr_gc_scanblack(object);
  }
}

static void hklr_gc_scanblack(HklObject* object)
{
  object->color = HKL_COLOR_BLACK;

  switch (object->type)
  {
    // If the object is a hash table
    case HKL_TYPE_HASH:
      // Traverse the hash scanning every child
      hkl_hash_traverse(object->as.hash, hklr_gc_scanblack_hash, NULL);
      break;

    // If the object is a reference
    case HKL_TYPE_REF:
      hklr_gc_scanblack(object->as.ref);
      break;

    default: break;
  }
}

// Foward declaration
static void hklr_gc_markgray(HklObject* object);
static void hklr_gc_markgray_hash(HklPair* pair, void* data)
{
  HklObject* object = pair->value;
  object->rc--;
  hklr_gc_markgray(object);
}

static void hklr_gc_markgray(HklObject* object)
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

      // If the object is a reference
      case HKL_TYPE_REF:

        object->as.ref->rc--;
        hklr_gc_markgray(object->as.ref);
        break;

      default: break;
    }
  }
}

static void hklr_gc_markroots()
{
  // For all possible roots
  HklObject* s = HKLR.gc_roots->next;
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
        HKLR.gc_freed++;
        hklr_object_free(s);
      }
    }
  }
}

// Foward declaration
static void hklr_gc_scan(HklObject* object);
static void hklr_gc_scan_hash(HklPair* pair, void* data)
{
  hklr_gc_scan((HklObject*) pair->value);
}

static void hklr_gc_scan(HklObject* object)
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
          hklr_gc_scan(object->as.ref);
          break;

        default: break;
      }
    }
  }
}

static void hklr_gc_scanroots()
{
  // For all possible roots
  HklObject* s = HKLR.gc_roots->next;
  for ( ; s != HKLR.gc_tail; s = s->next)
  {
    hklr_gc_scan(s);
  }
}

static void hklr_gc_collectwhite(HklObject* object);
static void hklr_gc_collectwhite_hash(HklPair* pair, void* data)
{
  hklr_gc_collectwhite((HklObject*) pair->value);
}

static void hklr_gc_collectwhite(HklObject* object)
{
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
        hklr_gc_collectwhite(object->as.ref);
        break;

      default: break;
    }

    HKLR.gc_freed++;
    hklr_object_free(object);
  }
}

static void hklr_gc_collectroots()
{
  // For all possible roots
  HklObject* s = HKLR.gc_roots->next;
  for ( ; s != HKLR.gc_tail; s = s->next)
  {
    // Remove node from possible roots
    s->prev->next = s->next;
    s->next->prev = s->prev;

    HKLR.gc_rootsize--;

    s->is_buffered = false;
    hklr_gc_collectwhite(s);
  }
}

void hklr_gc_collect()
{
  hklr_gc_markroots();
  hklr_gc_scanroots();
  hklr_gc_collectroots();

  HKLR.gc_runs++;
}
