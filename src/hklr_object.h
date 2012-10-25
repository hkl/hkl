#ifndef HKLR_OBJECT_H
#define HKLR_OBJECT_H

#include <stdint.h>

#include "hkl_string.h"
#include "hkl_hash.h"

typedef enum HklColor
{
  HKL_COLOR_BLACK,
  HKL_COLOR_GRAY,
  HKL_COLOR_WHITE,
  HKL_COLOR_PURPLE,
  HKL_COLOR_GREEN

} HklColor;

typedef enum HklFlags
{
  HKL_FLAG_NONE     = 0x000000,
  HKL_FLAG_CONST    = 0x000001,
  HKL_FLAG_UNIQUE   = 0x000002

} HklFlags;

typedef enum HklTypes
{

  HKL_TYPE_NONE,
  HKL_TYPE_CLASS,
  HKL_TYPE_INSTANCE,
  HKL_TYPE_STRING,
  HKL_TYPE_HASH,
  HKL_TYPE_ARRAY,
  HKL_TYPE_INT,
  HKL_TYPE_REAL,
  HKL_TYPE_FUNCTION,
  HKL_TYPE_NIL,
  HKL_TYPE_NULL,
  HKL_TYPE_REF,
  HKL_TYPE_CFUNC,
  HKL_TYPE_CDATA

} HklTypes;

/**
@struct HklObject The top-level object used by the HKLR

@authors Barrett Lewis, Scott LaVigne 
@date 10/8/2012
*/
typedef struct HklObject {

  // Used for garbage collection
  struct HklObject* prev;
  struct HklObject* next;
  uint32_t rc:          28;
  uint32_t is_buffered: 1;
  HklColor color:       3;

  HklTypes type:        24;
  HklFlags flags:       8;

  union asobject {

    uint32_t          integer;
    double            real;
    HklString*        string;
    HklHash*          hash;
    struct HklObject* ref;
  
  } as;

} HklObject;

/**
Allocate a new HklObject.

@param type The object type.
@param flags The informative flags for the object.
@param value An optional value to hand off to the object.
Passing NULL will result in a default allocation of the object of
the chosen type.

@brief WARNING: values passed as data to be contained in the object
will be owned by the object and will therefore be automatically
garbage collected by the HKLR.
*/
HklObject* hklr_object_new(HklTypes type, HklFlags flags, void* value);

/**
Free resources used by a HklObject

@param object The object to free.

@breif WARNING: This should essentially never be called by a user.
objects created by hkl_object_new will attempt to communicate
with the HKLR for automatic memory management.
*/
void hklr_object_free(HklObject* object);

/**
Create a reference link between two objects.

This function is mostly unimplemented.

If the child to reference is also a reference, there is a garuntee
that a link will be made to a leaf object. Chaining references can
cause long stalls in the HKLR.

@param object The parent object.
@param reference The child.
*/
void hklr_reference(HklObject* object, HklObject* reference);

/**
Add a child to a HklObject.

This function is mostly unimplemented.

This function is intended for mutating hklhash-based objects at runtime.
This should work for both hashes and classes

This should follow basic rules for evaluation.
There should be a garuntee that if value is a composite object, a reference to
it is stored.

@param object The parent object.
@param name An object to use as a key.
@param value An object to insert.
*/
void hklr_member_insert(HklObject* object, HklObject* name, HklObject* value);

#endif // HKL_OBJECT_H
