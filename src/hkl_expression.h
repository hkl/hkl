#ifndef HKL_EXPRESSION_H
#define HKL_EXPRESSION_H

#include "hkl_type.h"
#include "hkl_string.h"
#include "hkl_variable.h"

typedef enum HklExpressionContent
{
  HKL_EXPR_VARIABLE = 0x0,
  HKL_EXPR_CONSTANT = 0x1,
  HKL_EXPR_BINARY = 0x2,
  HKL_EXPR_UNARY = 0x4
} HklExpressionContent;

/**
@struct HklExpression A container for expressions in the hkl grammar.

@authors Chris Knadler
@date 9/15/2012
*/
typedef struct HklExpression HklExpression;

struct HklExpression
{
  HklExpression* expr_left;
  HklExpression* expr_right;
  void* value;
  HklType type;
  HklOperator op;
  HklExpressionContent content;
};

/**
Allocate a new unary HklExpression.

@param expr The expression for the unary operator to operate on.
@param op The operator.
@retval HklExpression* A new unary HklExpression.
@breif Allocates a new unary HklExpression.
*/
HklExpression* hkl_expression_new_unary_expression(HklExpression* expr, HklOperator op);

/**
Allocate a new binary HklExpression.

@param expr_left The left side of the expression.
@param expr_right The right side of the expression.
@param op The operator.
@retval HklExpression* A new binary HklExpression.
@brief Allocates a new binary HklExpression.
*/
HklExpression* hkl_expression_new_binary_expr(HklExpression* expr_left, HklExpression* expr_right, HklOperator op);

/**
Allocate a new constant HklExpression.

@param string_constant The constant to be stored as an HklExpression.
@retval HklExpression* A new constant HklExpression.
@brief Allocates a new real constant HklExpression.
*/
HklExpression* hkl_expression_new_constant_expr(void* constant);

/**
Allocate a new variable HklExpression.

@param variable The variable to be stored as an HklExpression.
@retval HklExpression* A new variable HklExpression.
@brief Allocates a new variable HklExpression.
*/
HklExpression* hkl_expression_new_variable_expr(HklVariable* variable);

/**
Erase the contents of a HklExpression

@param expr The HklExpression to clear.
*/
void hkl_expression_clear(HklExpression* expr);

/**
Free an HklExpression and release any allocated resources it has.

@param expr The HklExpression to free.
*/
void hkl_expression_free(HklExpression* expr);

#endif
