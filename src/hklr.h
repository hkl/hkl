#ifndef HKLR_H
#define HKLR_H

#include "hkl_object.h"

typedef struct HklRuntime
{
  HklObject* gc_roots;
  HklObject* gc_tail;
  size_t gc_runs;
  size_t gc_freed;

} HklRuntime;

HklRuntime HKLR;

void hklr_init();
void hklr_shutdown();

void hklr_gc_inc(HklObject* object);
void hklr_gc_dec(HklObject* object);
void hklr_gc_collect();

#endif // HKLR_H