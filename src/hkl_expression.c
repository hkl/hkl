#include <assert.h>

#include "hkl_expression.h"
#include "hkl_alloc.h"

HklExpression* hkl_expression_new_unary_expression(HklExpression* expr, HklOperator op)
{
  HklExpression* unary_expression = hkl_alloc_object(HklExpression);
  assert(unary_expression != NULL);

  assert(expr != NULL);

  // Make sure the operator is for a unary expression
  assert(op & HKL_UNARY_OP); 

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

  // Make sure the operator is a binary operator
  assert(op & HKL_BINARY_OP);

  binary_expression->expr_left = expr_left;
  binary_expression->expr_right = expr_right;
  binary_expression->op = op;
  binary_expression->content = HKL_EXPR_BINARY;

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

void hkl_expression_clear(HklExpression* expr)
{
   assert(expr != NULL);
 
   if (expr->content == HKL_EXPR_BINARY)
   {
     hkl_expression_free(expr->expr_right);
     expr->expr_right = NULL;

     hkl_expression_free(expr->expr_left);
     expr->expr_left = NULL;
   }

   if (expr->content == HKL_EXPR_UNARY)
   {
     hkl_expression_free(expr->expr_left);
     expr->expr_left = NULL;
   }

   if (expr->content == HKL_EXPR_VARIABLE)
   {
     hkl_variable_free(expr->value);
     expr->value = NULL;
   }

   if (expr->content == HKL_EXPR_CONSTANT)
   {
     free(expr->value);
     expr->value = NULL;
   }

   expr->type = 0;
   expr->op = 0;
   expr->content = 0;
}

void hkl_expression_free(HklExpression* expr)
{
  assert(expr != NULL);

  hkl_expression_clear(expr);

  free(expr);
}
