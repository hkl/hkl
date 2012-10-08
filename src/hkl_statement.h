#ifndef HKL_STATEMENT_H
#define HKL_STATEMENT_H

#include <stdarg.h>
#include <stddef.h>

#include "hkl_expression.h"

typedef enum HklStatementType
{
  HKL_STMT_NOOP,
  HKL_STMT_PUTS

} HklStatementType;

typedef struct HklStatement
{
  HklStatementType type;

  union arg {
  
    HklExpression* expression;

  } arg[4];

} HklStatement;

HklStatement* hkl_statement_new(HklStatementType type, ...);

void hkl_statement_exec(HklStatement* stmt);

#endif // HKL_STATEMENT_H