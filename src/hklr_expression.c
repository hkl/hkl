#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "hklr.h"
#include "hkl_alloc.h"
#include "hklr_expression.h"
#include "hklr_statement.h"
#include "hklr_function.h"
#include "hkl_variable.h"

extern void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs);

extern HklValue* hklr_op_size(HklValue* value);
extern HklValue* hklr_op_typeof(HklValue* value);
extern HklValue* hklr_as_integer(HklValue* value);
extern HklValue* hklr_as_real(HklValue* value);
extern HklValue* hklr_as_string(HklValue* value);

extern HklValue* hklr_op_plus(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_minus(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_less(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_less_equal(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_greater(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_greater_equal(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_multiply(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_divide(HklValue* left_value, HklValue* right_value);
extern HklValue* hklr_op_mod(HklValue* left_value, HklValue* right_value);
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

    case HKL_EXPR_TYPE:
      expr->arg[0].type = va_arg(argp, HklType);
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

    case HKL_EXPR_HASH:
      expr->arg[0].list = va_arg(argp, HklList*);
      break;

    case HKL_EXPR_FUNCTION:
      expr->arg[0].list = va_arg(argp, HklList*); // args
      expr->arg[1].tree = va_arg(argp, HklTree*); // closures
      expr->arg[2].list = va_arg(argp, HklList*); // stmts
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

static bool hklr_array_add_list(void* expr, void* array)
{
  HklValue* value = hklr_expression_eval((HklrExpression*) expr);

  hkl_deque_push_back((HklDeque*) array, value);

  return false;
}

static bool hklr_hash_add_list(void* pair, void* hash)
{
  HklrObject* object = hklr_object_new(HKL_TYPE_NIL, HKL_FLAG_NONE);

  hklr_object_assign(object, (HklrExpression*) ((HklPair*) pair)->value);

  hkl_hash_insert((HklHash*) hash, ((HklPair*) pair)->key, object);

  return false;
}

static bool hklr_statement_exec_list(void* stmt, void* data)
{
  int status = hklr_statement_exec((HklrStatement*) stmt);

  // Return from the function
  if (status == 3)
    return true;

  return false;
}

// This is used to carry a temporary scope to allow
// locals in a yet to be pushed scope to be defined ahead of time
struct scope_carier {

  HklListNode* arg;
  HklHash* locals;
  HklHash* upvals;

};

static bool make_locals(void* string, void* carrier)
{
  HklrObject* object = hklr_object_new(HKL_TYPE_NIL, HKL_FLAG_NONE);

  //hklr_local_insert((HklString*) string, object);
  hkl_hash_insert(((struct scope_carier*) carrier)->locals, (HklString*) string, object);

  // If you have too few args then the rest are nil
  if (((struct scope_carier*) carrier)->arg == NULL)
    return true;

  // Some fancy pointer arithematic
  // This iterates the argument expression
  HklrExpression* assign = ((struct scope_carier*) carrier)->arg->data;
  ((struct scope_carier*) carrier)->arg = ((struct scope_carier*) carrier)->arg->next;

  hklr_object_assign(object, assign);

  return false;
}

static bool make_closures(HklPair* pair, void* carrier)
{
  // create upvals for objects that are actual closures
  if (pair->value != NULL)
  {
    hkl_hash_insert(((struct scope_carier*) carrier)->upvals, pair->key, pair->value);
    //hklr_upval_insert(pair->key, pair->value);
  }

  return false;
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

    case HKL_EXPR_TYPE:
      return hkl_value_new(HKL_TYPE_TYPE, expr->arg[0].type);
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

      HklList* list = expr->arg[1].list;

      if (list->size && object->type != HKL_TYPE_REF)
        assert(false);

      HklListNode* node = list->head;
      while (node && object->type == HKL_TYPE_REF)
      {
        HklVariable* var = node->data;

        if (object->type == HKL_TYPE_REF && object->as.object->type == HKL_TYPE_FUNCTION)
        {
          HKLR.reg_return = NULL;

          // Try a function call
          HklrFunction* function = object->as.object->as.function;
          assert(var->type == HKL_VAR_CALL);
          HklList* args = var->as.list;

          struct scope_carier carrier;
          carrier.locals = hkl_hash_new();
          carrier.upvals = hkl_hash_new();
          carrier.arg = args->head;

          // Create the closure variables
          hkl_tree_traverse(function->closure_list, make_closures, &carrier);

          // Make the args in the function signature local variables
          hkl_list_traverse(function->args_list, make_locals, &carrier);

          hklr_scope_push_full(true, carrier.locals, carrier.upvals);

          // execute the statements within
          hkl_list_traverse(function->stmt_list, hklr_statement_exec_list, NULL);

          // the post_object is the function return value
          if (HKLR.reg_return == NULL)
            HKLR.reg_return = hkl_value_new(HKL_TYPE_NIL);

          // if the return value is an object be sure to increase the ref count
          // Though, this count needs to be decced afterwards
          if (HKLR.reg_return->type == HKL_TYPE_REF)
            hklr_gc_inc(HKLR.reg_return->as.object);

          hklr_scope_pop();

          return HKLR.reg_return;
        }
        else if (object->type == HKL_TYPE_REF && object->as.object->type == HKL_TYPE_HASH)
        {
          HklPair* pair = hkl_hash_search(object->as.object->as.hash, var->as.string);

          // This key doesnt exist just create it
          if (pair == NULL)
          {
            HklrObject* post_object = hklr_object_new(HKL_TYPE_NIL, HKL_FLAG_NONE);
            hkl_hash_insert(object->as.object->as.hash, var->as.string, post_object);

            return hkl_value_new(HKL_TYPE_REF, post_object);
          }

          if (node->next == NULL)
            return hkl_value_new(HKL_TYPE_REF, pair->value);

          object = pair->value;
        }

        node = node->next;
      }

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

    case HKL_EXPR_HASH:
    {
      HklHash* hash = hkl_hash_new();

      hkl_list_traverse(expr->arg[0].list, hklr_hash_add_list, hash);

      return hkl_value_new(HKL_TYPE_HASH, hash);
    }
    break;

    case HKL_EXPR_FUNCTION:
    {
      return hkl_value_new(HKL_TYPE_FUNCTION, hklr_function_new(expr->arg[0].list, expr->arg[1].tree, expr->arg[2].list));
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
              value->as.integer = -value->as.integer;
              return value;
              break;

            case HKL_TYPE_REAL:
              value->as.real = -value->as.real;
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

        case HKL_OP_TYPEOF:
          return hklr_op_typeof(value);
          break; // HKL_OP_TYPE

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
        case HKL_OP_MOD:
          result = hklr_op_mod(left_value, right_value);
          break;
        case HKL_OP_EQUAL:
          result = hklr_op_equal(left_value, right_value);
          break;
        case HKL_OP_AS:
        {
          assert(right_value->type == HKL_TYPE_TYPE);

          switch (right_value->as.type)
          {
            case HKL_TYPE_INT:
              result = hklr_as_integer(left_value);
            break;

            case HKL_TYPE_REAL:
              result = hklr_as_real(left_value);
            break;

            case HKL_TYPE_STRING:
              result = hklr_as_string(left_value);
            break;

            default:
              assert(false);
            break;
          }

        }
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

static bool hklr_array_free_list(void* expr, void* data)
{
  hklr_expression_free(expr);

  return false;
}

static bool hklr_hash_free_list(void* pair, void* data)
{
  // each item contains a string expression pair

  hklr_expression_free((HklrExpression*) ((HklPair*) pair)->value);
  hkl_pair_free((HklPair*) pair);

  return false;
}

static bool hklr_var_free_list(void* var, void* data)
{
  // each item contains a var

  hkl_variable_free((HklVariable*) var);

  return false;
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
      hkl_list_traverse(expr->arg[1].list, hklr_var_free_list, NULL);
      hkl_list_free(expr->arg[1].list);
      break;
      
    case HKL_EXPR_ARRAY:
     hkl_list_traverse(expr->arg[0].list, hklr_array_free_list, NULL);
     hkl_list_free(expr->arg[0].list);
     break;

    case HKL_EXPR_HASH:
     hkl_list_traverse(expr->arg[0].list, hklr_hash_free_list, NULL);
     hkl_list_free(expr->arg[0].list);
     break;

    default:
      break;
  }

  hkl_free_object(expr);
}
