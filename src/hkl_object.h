#ifndef HKL_OBJECT_H
#define HKL_OBJECT_H

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

typedef struct HklObject {

  // Used for garbage collection
  struct HklObject* prev;
  struct HklObject* next;
  uint32_t rc:          28;
  uint32_t is_buffered: 1;
  HklColor color:       3;

  HklTypes type:        24;
  HklFlags flags:       8;

  union {

    uint32_t          integer;
    double            real;
    HklString*        string;
    HklHash*          hash;
    struct HklObject* ref;
  
  };

} HklObject;

HklObject* hkl_object_new();

void hkl_object_free(HklObject* object);

#endif // HKL_OBJECT_H
