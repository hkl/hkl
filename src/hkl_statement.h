#ifndef HKL_STATEMENT_H
#define HKL_STATEMENT_H

#include <stdarg.h>
#include <stddef.h>

#include "hkl_expression.h"

typedef enum HklStatementType
{
  HKL_STMT_NONE,
  HKL_STMT_PUTS,
  HKL_STMT_HKLR

} HklStatementType;

typedef struct HklStatement
{
  HklStatementType type;

  union stmtarg {
  
    HklString* id;
    HklExpression* expression;

  } arg[2];

} HklStatement;

HklStatement* hkl_statement_new(HklStatementType type, ...);

void hkl_statement_exec(HklStatement* stmt);

void hkl_statement_free(HklStatement* stmt);

#endif // HKL_STATEMENT_H
