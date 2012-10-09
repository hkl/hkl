#ifndef HKL_EXPRESSION_H
#define HKL_EXPRESSION_H

#include <stdarg.h>

#include "hkl_string.h"

typedef enum HklExpressionType {

  HKL_EXPR_INT,
  HKL_EXPR_REAL,
  HKL_EXPR_STRING,
  HKL_EXPR_GETS

} HklExpressionType;

typedef struct HklExpression
{
  HklExpressionType type;

  union exprarg {
  
    int integer;
    double real;
    HklString* string;

  } arg[1];

} HklExpression;

HklExpression* hkl_expression_new(HklExpressionType type, ...);

HklString* hkl_expression_eval_string(HklExpression* expr);

void hkl_expression_free(HklExpression* expr);
  
#endif // HKL_EXPRESSION_H