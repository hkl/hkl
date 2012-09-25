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
  HKL_FLAG_NONE,
  HKL_FLAG_INT,
  HKL_FLAG_REAL,
  HKL_FLAG_STRING,
  HKL_FLAG_HASH,
  HKL_FLAG_REF

} HklFlags;

typedef struct HklObject {

  // Used for garbage collection
  struct HklObject* prev;
  struct HklObject* next;
  uint32_t rc:          28;
  uint32_t is_buffered: 1;
  HklColor color:       3;

  HklFlags flags;

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