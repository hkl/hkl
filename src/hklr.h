#ifndef HKLR_H
#define HKLR_H

#include "hklr_object.h"

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
  size_t gc_runs;
  size_t gc_created;
  size_t gc_freed;
  size_t gc_rootsize;

  HklHash* globals;
  HklScope* scopes;

} HklRuntime;

HklRuntime HKLR;

void hklr_init();
void hklr_shutdown();

void hklr_gc_inc(HklObject* object);
void hklr_gc_dec(HklObject* object);
void hklr_gc_collect();

void hklr_scope_push();
void hklr_scope_pop();

#endif // HKLR_H
