#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hklr.h"
#include "hkl_alloc.h"
#include "hklr_expression.h"
#include "hklr_statement.h"

extern void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs);

extern HklValue* hklr_op_size(HklValue* value);
extern HklValue* hklr_op_plus(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_minus(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_less(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_less_equal(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_greater(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_greater_equal(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_multiply(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_divide(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_not_equal(HklValue* left_value, HklValue* right_value);
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

    case HKL_EXPR_VAR:
      expr->arg[0].string = va_arg(argp, HklString*);
      expr->arg[1].list = va_arg(argp, HklList*);
      break;

    case HKL_EXPR_ARRAY:
      expr->arg[0].list = va_arg(argp, HklList*);
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

static void hklr_array_add_list(void* expr, void* array)
{
  HklValue* value = hklr_expression_eval((HklrExpression*) expr);

  hkl_deque_push_front((HklDeque*) array, value);
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

    case HKL_EXPR_VAR:
    {
      HklrObject* object = hklr_search(expr->arg[0].string);

      // apply more list items to the object to fetch deeper ones

      return hkl_value_new(HKL_TYPE_REF, object);
    }
    break;

    case HKL_EXPR_GETS:
    {
      HklString* string = hkl_string_new_from_stream(stdin);
      assert(string != NULL);
      return hkl_value_new(HKL_TYPE_STRING, string);
    }
    break;

    case HKL_EXPR_ARRAY:
    {
      // allocate space ahead of time
      HklDeque* deque = hkl_deque_new();//_sized(list->size);
      hkl_list_traverse(expr->arg[0].list, hklr_array_add_list, deque);

      return hkl_value_new(HKL_TYPE_ARRAY, deque);
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
        case HKL_OP_MINUS:
          result = hklr_op_minus(left_value, right_value);
          break;
        case HKL_OP_LESS:
          result = hklr_op_less(left_value, right_value);
          break;  
        case HKL_OP_LESS_EQUAL:
          result = hklr_op_less_equal(left_value, right_value);
          break;   
        case HKL_OP_GREATER:
          result = hklr_op_greater(left_value, right_value);
          break;  
        case HKL_OP_GREATER_EQUAL:
          result = hklr_op_greater_equal(left_value, right_value);
          break;
        case HKL_OP_ASTERISK: 
          result = hklr_op_multiply(left_value, right_value); 
          break;
        case HKL_OP_DIVIDE:
          result = hklr_op_divide(left_value, right_value);
          break;
        case HKL_OP_EQUAL:
          result = hklr_op_equal(left_value, right_value);
          break;
        case HKL_OP_ASSIGN:
          hklr_statement_assign(expr->arg[0].expression, expr->arg[2].expression);
          // This technically doesnt count as an op
          result = hklr_expression_eval(expr->arg[0].expression);
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

    default:
    assert(false);
      break;
  }

  assert(false);
  return NULL;
}

static void hklr_array_free_list(void* expr, void* data)
{
  hklr_expression_free(expr);
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

    case HKL_EXPR_VAR:

      hkl_string_free(expr->arg[0].string);
      hkl_list_free(expr->arg[1].list);
      break;
      
    case HKL_EXPR_ARRAY:
     hkl_list_traverse(expr->arg[0].list, hklr_array_free_list, NULL);
     hkl_list_free(expr->arg[0].list);
     break;

    default:
      break;
  }

  hkl_free_object(expr);
}
