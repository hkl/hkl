#ifndef HKL_ALLOC_H
#define HKL_ALLOC_H

#include <stdlib.h>

#define hkl_alloc_object(type) ((type *) malloc(sizeof(type)))

#endif // HKL_ALLOC_H
