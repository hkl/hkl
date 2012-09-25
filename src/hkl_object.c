#include "hkl_alloc.h"

#include "hkl_object.h"

HklObject* hkl_object_new()
{
  HklObject* object = hkl_alloc_object(HklObject);

  object->prev = NULL;
  object->next = NULL;
  object->rc = 0;
  object->color = HKL_COLOR_BLACK;

  object->flags = HKL_FLAG_NONE;

  return object;
}

void hkl_object_free(HklObject* object)
{
  hkl_free_object(object);
}