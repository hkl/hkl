#ifndef HKL_VALUE_H
#define HKL_VALUE_H

#include <stdarg.h>

#include "hkl_string.h"
#include "hkl_hash.h"
#include "hklr_object.h"

typedef struct HklValue 
{

  HklTypes type:  22;
  HklFlags flags: 8;

  union asvalue {

    uint32_t          integer;
    double            real;
    HklString*        string;
    HklHash*          hash;
    struct HklObject* ref;

  } as;

} HklValue;

HklValue* hkl_value_new(HklTypes type, ...);

void hkl_value_free(HklValue* value);

#endif
