#ifndef HKLR_H
#define HKLR_H

#include "hklr_object.h"
#include "hkl_deque.h"

typedef struct HklScope {

  struct HklScope* prev;
  struct HklScope* next;

  HklHash* locals;
  HklHash* upvals;

} HklScope;

typedef struct HklRuntime
{
  HklObject* gc_roots;
  HklObject* gc_tail;
  HklDeque* gc_to_free; // objects waiting to be freed
  size_t gc_runs;
  size_t gc_created;
  size_t gc_freed;
  size_t gc_rootsize;

  HklHash* globals;
  HklScope* scopes;
  size_t scope_level;

} HklRuntime;

HklRuntime HKLR;

void hklr_init();
void hklr_shutdown();

void hklr_gc_inc(HklObject* object);
void hklr_gc_dec(HklObject* object);
void hklr_gc_collect();

void hklr_scope_push();
void hklr_scope_pop();

// These are explicit insertions that WILL override the HKLR's
// "best efforts" to properly place a variable for you  
void hklr_local_insert(HklString* key, HklObject* value);
void hklr_upval_insert(HklString* key, HklObject* value);
void hklr_global_insert(HklString* key, HklObject* value);

// You are given the nearest object of name, key
// if the object of name, key is a ref you get what
// it is referencing
HklObject* hklr_search(HklString* key);

#endif // HKLR_H
