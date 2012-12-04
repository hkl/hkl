#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hkl_value.h"
#include "hklr_expression.h"

double hklapi_PI = 3.1415926;
double hklapi_TAU = 2.0*3.1415926;

HklValue* hklapi_random(HklList* args) {
  
  return hkl_value_new(HKL_TYPE_INT, random());
}

HklValue* hklapi_sin(HklList* args) {
  
  HklValue* value = args->head->data;
  double d = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, sin(d));
}

HklValue* hklapi_cos(HklList* args) {
  
  HklValue* value = args->head->data;
  double d = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, cos(d));
}

HklValue* hklapi_tan(HklList* args) {
  
  HklValue* value = args->head->data;
  double d = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, tan(d));
}

HklValue* hklapi_log(HklList* args) {
  
  HklValue* value = args->head->data;
  double d = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, log(d));
}

HklValue* hklapi_log10(HklList* args) {
  
  HklValue* value = args->head->data;
  double d = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, log10(d));
}

HklValue* hklapi_pow(HklList* args) {
  
  HklValue* value = args->head->data;
  double base = value->as.real;
  hkl_value_free(value);


  value = args->head->next->data;
  double e = value->as.real;
  hkl_value_free(value);

  return hkl_value_new(HKL_TYPE_REAL, pow(base, e));
}