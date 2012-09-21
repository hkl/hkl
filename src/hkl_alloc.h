#ifndef HKL_ALLOC_H
#define HKL_ALLOC_H

#include <stdlib.h>

#define hkl_alloc_object(type) ((type *) malloc(sizeof(type)))

#define hkl_free_object(obj) (free(obj))

#endif // HKL_ALLOC_H
