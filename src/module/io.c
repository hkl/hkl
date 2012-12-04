#include <stdio.h>
#include <assert.h>

#include "hkl_value.h"
#include "hkl_string.h"

HklValue* hklapi_open(HklList* args)
{
  HklValue* path = args->head->data;
  HklValue* mode = args->head->next->data;

  FILE* fd = fopen(path->as.string->utf8_data, mode->as.string->utf8_data);
  assert(fd);

  return hkl_value_new(HKL_TYPE_CDATA, fd);
}

HklValue* hklapi_write(HklList* args)
{
  HklValue* fd = args->head->data;

  assert(fd->type == HKL_TYPE_CDATA);

  HklValue* string = args->head->next->data;
  assert(string->type == HKL_TYPE_STRING);

  fprintf(fd->as.cdata, "%s", string->as.string->utf8_data);

  return NULL;
}

HklValue* hklapi_close(HklList* args)
{
  HklValue* fd = args->head->data;

  assert(fd->type == HKL_TYPE_CDATA);

  fclose(fd->as.cdata);

  return NULL;
}