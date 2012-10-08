#include "hkl_alloc.h"
#include "hkl_expression.h"

HklExpression* hkl_expression_new(HklExpressionType type, ...)
{
  HklExpression* expr = hkl_alloc_object(HklExpression);


  return expr;
}