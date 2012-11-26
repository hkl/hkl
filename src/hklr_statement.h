#ifndef HKLR_STATEMENT_H
#define HKLR_STATEMENT_H

#include <stdarg.h>
#include <stddef.h>

#include "hklr_expression.h"
#include "hkl_list.h"

typedef enum HklStatementType
{
  HKL_STMT_NONE,
  HKL_STMT_PUTS,
  HKL_STMT_HKLR,
  HKL_STMT_ASSIGN,
  HKL_STMT_IF,
  HKL_STMT_WHILE,
  HKL_STMT_COLLECT,
  HKL_STMT_CALL

} HklStatementType;

typedef struct HklrStatement
{
  HklStatementType type;

  union stmtarg {
  
    HklrExpression* expression;
    HklList*        list;
    HklString*      string;
    HklFlag         flags;

  } arg[3];

} HklrStatement;

HklrStatement* hklr_statement_new(HklStatementType type, ...);

void hklr_statement_exec(HklrStatement* stmt);

void hklr_statement_free(HklrStatement* stmt);

#endif // HKLR_STATEMENT_H
