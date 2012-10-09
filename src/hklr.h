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

/**
@struct The hkl Runtime

This is the "state" of the state machine. This structure manages all
the throughput of the language.

@authors Barrett Lewis, Scott LaVigne 
@date 10/8/2012
*/
typedef struct HklRuntime
{
  HklObject* gc_roots;  // Dummy nodes used for bacon gc
  HklObject* gc_tail;
  HklDeque* gc_to_free; // Cyclic roots queued for collection
  size_t gc_runs;       // Number of times gc has ran
  size_t gc_created;    // Number of objects the gc has created
  size_t gc_freed;      // Number of objects the gc has freed
  size_t gc_rootsize;   // The number of cyclic roots queued for baconing

  HklHash* globals;     // Runtime global variables list
  HklScope* scopes;     // Runtime stack frams
  size_t scope_level;   // Current scope depth

} HklRuntime;

HklRuntime HKLR;        // Instanciate a single HKLR thread

/**
Initialize the HKLR.

This function sets the HKLR defaults, pushes the initial stack frame and
primes bacon for collecting garbage.
*/
void hklr_init();

/**
Shutdown the HKLR.

Halts hkl execution and cleans up all remaining garbage queued in the runtime.
*/
void hklr_shutdown();

/**
Increment the reference count manually on an object.

Requests bacon to increment the reference count of an object.

@param object The object to mutate.
*/
void hklr_gc_inc(HklObject* object);

/**
Decrement the reference count manually on an object.

Requests bacon to decrement the reference count of an object.
This operation can be slow if done manually. Normally, this
will also free the object. Once ran you have no garuntee the
object still exists.

@param object The object to mutate.
*/
void hklr_gc_dec(HklObject* object);

/**
Initializes collection of cyclic nodes.

This requests bacon to make a best efforts collection
of nodes.
*/
void hklr_gc_collect();

/**
Push a new HKLR stack frame.

Creates a new stack frame in the HKLR and initializes it
as the current context.
*/
void hklr_scope_push();

/**
Pop the current HKLR stack frame.

All local HklObjects that existed before this
call may or may not continue to exist.
*/
void hklr_scope_pop();

/**
Manually force a variable into the local context.

This function will move a varibale into the "local scope".
Once you hand off the object, it will be managed by the HKLR.

@breif WARNING: This will annihilate the reference to any object that existed
before with the same key. The node will become unreachable and will never be
collected. It is probably best to use this in conjunction with hklr_search.

@param key The name of the object.
@param value An object to make local.
*/ 
void hklr_local_insert(HklString* key, HklObject* value);

/**
Manually force a variable into the upper context.

This function will move a varibale into the "upper scope".
Once you hand off the object, it will be managed by the HKLR.

@breif WARNING: This will annihilate the reference to any object that existed
before with the same key. The node will become unreachable and will never be
collected. It is probably best to use this in conjunction with hklr_search.

@param key The name of the object.
@param value An object to make into an upval.
*/ 
void hklr_upval_insert(HklString* key, HklObject* value);

/**
Manually force a variable into the global context.

This function will move a varibale into the "global scope".
Once you hand off the object, it will be managed by the HKLR.

@breif WARNING: This will annihilate the reference to any object that existed
before with the same key. The node will become unreachable and will never be
collected. It is probably best to use this in conjunction with hklr_search.

@param key The name of the object.
@param value An object to make into a global.
*/ 
void hklr_global_insert(HklString* key, HklObject* value);

// You are given the nearest object of name, key
// if the object of name, key is a ref you get what
// it is referencing
HklObject* hklr_search(HklString* key);

#endif // HKLR_H
