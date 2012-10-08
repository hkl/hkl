#ifndef HKL_EXPRESSION_H
#define HKL_EXPRESSION_H

#include <stdarg.h>

typedef enum HklExpressionType {

  HKL_EXPR_INT,
  HKL_EXPR_REAL,
  HKL_EXPR_STRING

} HklExpressionType;

typedef struct HklExpression
{

} HklExpression;

HklExpression* hkl_expression_new(HklExpressionType type, ...);

#endif // HKL_EXPRESSION_H