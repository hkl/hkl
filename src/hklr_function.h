#ifndef HKLR_FUNCTION_H
#define HKLR_FUNCTION_H

#include "hkl_list.h"

typedef struct HklrFunction {

  HklList* stmt_list;
  HklList* args_list;
  HklList* closure_list;

} HklrFunction;

HklrFunction* hklr_function_new(HklList* args_list, HklList* closure_list, HklList* stmt_list);

void hklr_function_free(HklrFunction* function);

#endif // HKLR_FUNCTION_H