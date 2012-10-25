#ifndef HKL_EXPRESSION_H
#define HKL_EXPRESSION_H

#include <stdarg.h>

#include "hkl_string.h"
#include "hkl_value.h"

typedef enum HklExpressionType 
{
  HKL_EXPR_NONE,
  HKL_EXPR_INT,
  HKL_EXPR_REAL,
  HKL_EXPR_STRING,
  HKL_EXPR_GETS,
  HKL_EXPR_UNARY,
  HKL_EXPR_BINARY

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
  HKL_OP_SIZE

} HklOperatorType;

typedef struct HklExpression
{
  HklExpressionType type;

  union exprarg {
  
    int integer;
    double real;
    HklString* string;
    struct HklExpression* expression;
    HklOperatorType op;

  } arg[3];

} HklExpression;

HklExpression* hkl_expression_new(HklExpressionType type, ...);

HklValue* hkl_expression_eval(HklExpression* expr);

void hkl_expression_free(HklExpression* expr);
  
#endif // HKL_EXPRESSION_H
