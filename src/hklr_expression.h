#ifndef HKLR_EXPRESSION_H
#define HKLR_EXPRESSION_H

#include <stdarg.h>

#include "hkl_string.h"
#include "hkl_value.h"
#include "hkl_list.h"
#include "hklr_object.h"

typedef enum HklExpressionType 
{
  HKL_EXPR_NONE,
  HKL_EXPR_NIL,
  HKL_EXPR_MAYBE,
  HKL_EXPR_INT,
  HKL_EXPR_TYPE,
  HKL_EXPR_REAL,
  HKL_EXPR_STRING,
  HKL_EXPR_GETS,
  HKL_EXPR_UNARY,
  HKL_EXPR_BINARY,
  HKL_EXPR_VAR,
  HKL_EXPR_ARRAY,
  HKL_EXPR_HASH,
  HKL_EXPR_FUNCTION,
  HKL_EXPR_INTERFACE

} HklExpressionType;

typedef enum HklOperatorType 
{
  HKL_OP_NOT,
  HKL_OP_BITWISE_NOT,
  HKL_OP_INCREMENT,
  HKL_OP_DECREMENT,
  HKL_OP_UNARY_MINUS,
  HKL_OP_OR,
  HKL_OP_AND,
  HKL_OP_LESS_EQUAL,
  HKL_OP_GREATER_EQUAL,
  HKL_OP_LESS,
  HKL_OP_GREATER,
  HKL_OP_EQUAL,
  HKL_OP_NOT_EQUAL,
  HKL_OP_PLUS,
  HKL_OP_MINUS,
  HKL_OP_ASTERISK,
  HKL_OP_DIVIDE,
  HKL_OP_MOD,
  HKL_OP_BITWISE_AND,
  HKL_OP_BITWISE_OR,
  HKL_OP_BITWISE_XOR,
  HKL_OP_SIZE,
  HKL_OP_DOT,
  HKL_OP_INDEX,
  HKL_OP_CALL,
  HKL_OP_TYPEOF,
  HKL_OP_AS,
  HKL_OP_COALESCE

} HklOperatorType;

typedef struct HklrExpression
{
  HklExpressionType type;

  union exprarg {
  
    int integer;
    double real;
    HklString* string;
    struct HklrExpression* expression;
    HklOperatorType op;
    HklList* list;
    HklTree* tree;
    HklType type;

  } arg[3];

} HklrExpression;

HklrExpression* hklr_expression_new(HklExpressionType type, ...);

HklValue* hklr_expression_eval(HklrExpression* expr);

void hklr_expression_free(HklrExpression* expr);
  
#endif // HKLR_EXPRESSION_H
