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

typedef enum HklFlag
{
  HKL_FLAG_NONE     = 0x000000,
  HKL_FLAG_CONST    = 0x000001,
  HKL_FLAG_UNIQUE   = 0x000002

} HklFlag;

typedef enum HklType
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

} HklType;

/**
@struct HklrObject The top-level object used by the HKLR

@authors Barrett Lewis, Scott LaVigne 
@date 10/8/2012
*/
typedef struct HklrObject {

  // Used for garbage collection
  struct HklrObject* prev;
  struct HklrObject* next;
  uint32_t rc:          28;
  uint32_t is_buffered: 1;
  HklColor color:       3;

  HklType type:        24;
  HklFlag flags:       8;

  union asobject {

    uint32_t          integer;
    double            real;
    HklString*        string;
    HklHash*          hash;
    struct HklrObject* ref;
  
  } as;

} HklrObject;

/**
Allocate a new HklObject.

@param type The object type.
@param flags The informative flags for the object.
@param ... Optional value(s) to hand off to the object.
Passing NULL will result in a default allocation of the object of
the chosen type.

@brief WARNING: values passed as data to be contained in the object
will be owned by the object and will therefore be automatically
garbage collected by the HKLR.
*/
HklrObject* hklr_object_new(HklType type, HklFlag flags, ...);

/**
Free resources used by a HklObject

@param object The object to free.

@breif WARNING: This should essentially never be called by a user.
objects created by hkl_object_new will attempt to communicate
with the HKLR for automatic memory management.
*/
void hklr_object_free(HklrObject* object);

#endif // HKL_OBJECT_H
