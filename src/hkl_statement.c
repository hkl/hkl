#include <assert.h>

#include "hkl_alloc.h"
#include "hkl_statement.h"

HklStatement* hkl_statement_new(HklStatementType type, ...)
{
  HklStatement* stmt = hkl_alloc_object(HklStatement);
  stmt->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_STMT_PUTS:
    // puts requires 1 expression
    stmt->arg[0].expression = va_arg(argp, HklExpression*);

    break;

    default:
    break;
  }

  va_end(argp);

  return stmt;
}

void hkl_statement_exec(HklStatement* stmt)
{
  assert(stmt != NULL);

  switch (stmt->type)
  {
    case HKL_STMT_PUTS:
      
    break;

    default:
    break;
  }
}
