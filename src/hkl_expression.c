#include <assert.h>

#include "hkl_expression.h"
#include "hkl_alloc.h"

HklExpression* hkl_expression_new_unary_expression(HklExpression* expr, HklOperator op)
{
  HklExpression* unary_expression = hkl_alloc_object(HklExpression);
  assert(unary_expression != NULL);

  assert(expr != NULL);
  assert(op != NULL);

  // Make sure the operator is for a unary expression
  assert(op != HKL_OR);
  assert(op != HKL_AND);
  assert(op != HKL_LESS_EQUAL);
  assert(op != HKL_GREATER_EQUAL);
  assert(op != HKL_LESS);
  assert(op != HKL_GREATER);
  assert(op != HKL_EQUAL);
  assert(op != HKL_NOT_EQUAL);
  assert(op != HKL_PLUS);
  assert(op != HKL_MINUS);
  assert(op != HKL_ASTERISK);
  assert(op != HKL_DIVIDE);
  assert(op != HKL_MOD);
  assert(op != HKL_BITWISE_AND);
  assert(op != HKL_BITWISE_OR);
  assert(op != HKL_BITWISE_XOR);
  assert(op != HKL_TYPE_OF);
  assert(op != HKL_INSTANCE_OF);

  unary_expression->expr_left = expr;
  unary_expression->op = op;
  unary_expression->content = HKL_EXPR_UNARY;

  return unary_expression;
}

HklExpression* hkl_expression_new_binary_expression(HklExpression* expr_left, HklExpression* expr_right, HklOperator op)
{
  HklExpression* binary_expression = hkl_alloc_object(HklExpression);
  assert(binary_expression != NULL);

  assert(expr_left != NULL);
  assert(expr_right != NULL);
  assert(op != NULL);

  // Make sure the operator is a binary operator
  assert(op != HKL_NOT);
  assert(op != HKL_BITWISE_NOT);
  assert(op != HKL_INCRMENT);
  assert(op != HKL_DECREMENT);

  binary_expression->expr_left = expr_left;
  binary_expression->expr_right = expr_right;
  binary_expression->op = op;
  binary_experssion->content = HKL_EXPR_BINARY;

  return binary_expression;
}

HklExpression* hkl_expression_new_constant_expr(void* constant)
{
  HklExpression* constant_expr = hkl_alloc_object(HklExpression);
  assert(constant_expr != NULL);

  assert(constant != NULL);
  
  constant_expr->value = constant;
  constant_expr->content = HKL_EXPR_CONSTANT;

  return constant_expr;
}

HklExpression* hkl_expression_new_variable_expr(HklVariable* variable)
{
  HklExpression* variable_expr = hkl_alloc_object(HklExpression);
  assert(variable_expr != NULL);

  assert(variable != NULL);

  variable_expr->value = variable;
  variable_expr->content = HKL_EXPR_VARIABLE;

  return variable_expr;
}
