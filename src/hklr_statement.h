#ifndef HKLR_STATEMENT_H
#define HKLR_STATEMENT_H

#include <stdarg.h>
#include <stddef.h>

#include "hklr_expression.h"

typedef enum HklStatementType
{
  HKL_STMT_NONE,
  HKL_STMT_PUTS,
  HKL_STMT_HKLR,
  HKL_STMT_INIT,
  HKL_STMT_ASSIGN

} HklStatementType;

typedef struct HklrStatement
{
  HklStatementType type;

  union stmtarg {
  
    HklrExpression* expression;

  } arg[2];

} HklrStatement;

HklrStatement* hklr_statement_new(HklStatementType type, ...);

void hklr_statement_exec(HklrStatement* stmt);

void hklr_statement_free(HklrStatement* stmt);

#endif // HKLR_STATEMENT_H
