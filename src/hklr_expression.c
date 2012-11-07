#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hklr.h"
#include "hkl_alloc.h"
#include "hklr_expression.h"

extern HklValue* hklr_op_size(HklValue* value);
extern HklValue* hklr_op_plus(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_equal(HklValue* left_value, HklValue* right_value);

HklrExpression* hklr_expression_new(HklExpressionType type, ...)
{
  assert(type != HKL_EXPR_NONE);

  HklrExpression* expr = hkl_alloc_object(HklrExpression);

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

    case HKL_EXPR_ID:
      expr->arg[0].string = va_arg(argp, HklString*);
      break;

    case HKL_EXPR_UNARY:
      expr->arg[0].op = va_arg(argp, HklOperatorType);
      expr->arg[1].expression = va_arg(argp, HklrExpression*);
      break;
      
    case HKL_EXPR_BINARY:
      expr->arg[0].expression = va_arg(argp, HklrExpression*);
      expr->arg[1].op = va_arg(argp, HklOperatorType);
      expr->arg[2].expression = va_arg(argp, HklrExpression*);
      break;

    default:
      break;
  }

  va_end(argp);

  return expr;
}

HklValue* hklr_expression_eval(HklrExpression* expr)
{
  assert(expr != NULL);

  switch (expr->type)
  {
    case HKL_EXPR_NIL:
      return hkl_value_new(HKL_TYPE_NIL);
      break;

    case HKL_EXPR_MAYBE:
      return hkl_value_new(HKL_TYPE_INT, rand() % 2 == 1);
      break;

    case HKL_EXPR_INT:
      return hkl_value_new(HKL_TYPE_INT, expr->arg[0].integer);
      break;

    case HKL_EXPR_REAL:
      return hkl_value_new(HKL_TYPE_REAL, expr->arg[0].real);
      break;

    case HKL_EXPR_STRING: 
    {
      HklString* string = hkl_string_new_from_string(expr->arg[0].string);
      assert(string != NULL);
      return hkl_value_new(HKL_TYPE_STRING, string);
    }
    break;

    case HKL_EXPR_GETS:
    {
      HklString* string = hkl_string_new_from_stream(stdin);
      assert(string != NULL);
      return hkl_value_new(HKL_TYPE_STRING, string);
    }
    break;

    case HKL_EXPR_UNARY:
    {
      HklValue* value = hklr_expression_eval(expr->arg[1].expression);
      assert(value != NULL);

      switch (expr->arg[0].op)
      {
        case HKL_OP_UNARY_MINUS:
          switch (value->type)
          {
            case HKL_TYPE_INT:
              value->as.integer = -(value->as.integer);
              return value;
              break;

            case HKL_TYPE_REAL:
              value->as.real = -(value->as.real);
              return value;
              break;

            default:
              assert(false);
              break;
          }
          break; // HKL_OP_UNARY_MINUS

        case HKL_OP_SIZE:
          return hklr_op_size(value);
          break; // HKL_OP_SIZE

        default:
          assert(false);
          break;
      }
    }
    break; // HKL_EXPR_UNARY

    case HKL_EXPR_BINARY:
    {
      HklValue *left_value = hklr_expression_eval(expr->arg[0].expression);
      HklValue *right_value = hklr_expression_eval(expr->arg[2].expression);
      HklValue *result = NULL;

      assert(left_value != NULL);
      assert(right_value != NULL);

      switch (expr->arg[1].op)
      {
        case HKL_OP_PLUS:
          result = hklr_op_plus(left_value, right_value);
          break;

        case HKL_OP_EQUAL:
          result = hklr_op_equal(left_value, right_value);
          break;

        default:
          assert(false);
          break;
      }

      hkl_value_free(left_value);
      hkl_value_free(right_value);

      return result;
    }
    break; // HKL_EXPR_BINARY

    case HKL_EXPR_ID:
      return hkl_value_new(HKL_TYPE_REF, hklr_search(expr->arg[0].string));
    break;

    default:
    assert(false);
      break;
  }

  assert(false);
  return NULL;
}

void hklr_expression_free(HklrExpression* expr)
{
  assert(expr != NULL);

  switch (expr->type)
  {
    case HKL_EXPR_STRING:
      // free the internal string
      hkl_string_free(expr->arg[0].string);
      break;

    case HKL_EXPR_UNARY:
      hklr_expression_free(expr->arg[1].expression);
      break;

    case HKL_EXPR_BINARY:
      hklr_expression_free(expr->arg[0].expression);
      hklr_expression_free(expr->arg[2].expression);
      break;

    case HKL_EXPR_ID:
      hkl_string_free(expr->arg[0].string);
      break;

    default:
      break;
  }

  hkl_free_object(expr);
}
