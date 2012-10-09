#include <stdio.h>
#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_expression.h"

HklExpression* hkl_expression_new(HklExpressionType type, ...)
{
  HklExpression* expr = hkl_alloc_object(HklExpression);

  expr->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_EXPR_INT:
      expr->arg[0].integer = va_arg(argp, int);
      break;

    case HKL_EXPR_REAL:
      expr->arg[0].real = va_arg(argp, double);
      break;

    case HKL_EXPR_STRING:
      expr->arg[0].string = va_arg(argp, HklString*);
      break;

    default:
    break;
  }

  va_end(argp);

  return expr;
}

HklString* hkl_expression_eval_string(HklExpression* expr)
{
  assert(expr != NULL);

  switch (expr->type)
  {
    case HKL_EXPR_STRING:
      // return a copy of the string
      return hkl_string_new_from_string(expr->arg[0].string);
    break;

    case HKL_EXPR_INT:
      return hkl_string_new_from_integer(expr->arg[0].integer);
    break;

    case HKL_EXPR_REAL:
      return hkl_string_new_from_real(expr->arg[0].real);
    break;

    case HKL_EXPR_GETS:
      return hkl_string_new_from_stream(stdin);
    break;

    default:
    break;
  }

  assert(false);
  return NULL;
}

void hkl_expression_free(HklExpression* expr)
{
  assert(expr != NULL);

  switch (expr->type)
  {
    case HKL_EXPR_STRING:
      // free the internal string
      hkl_string_free(expr->arg[0].string);
    break;

    default:
    break;
  }

  hkl_free_object(expr);
}