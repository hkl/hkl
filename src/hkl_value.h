#ifndef HKL_VALUE_H
#define HKL_VALUE_H

#include <stdarg.h>

#include "hkl_string.h"
#include "hkl_hash.h"
#include "hklr_object.h"
#include "hkl_deque.h"

typedef struct HklValue 
{

  HklType type:  24;
  HklFlag flags: 8;

  union asvalue {

    uint32_t           integer;
    double             real;
    HklString*         string;
    HklHash*           hash;
    HklDeque*          deque;
    HklrObject*        object;
    HklType            type;

  } as;

} HklValue;

HklValue* hkl_value_new(HklType type, ...);

void hkl_value_free(HklValue* value);

#endif
