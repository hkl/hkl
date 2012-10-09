#include <assert.h>
#include <stdio.h>

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
    {
      HklString* string = hkl_expression_eval_string(stmt->arg[0].expression);
      fprintf(stdout, "%s", string->utf8_data);
      hkl_string_free(string);
    }
    break;

    default:
    break;
  }
}

void hkl_statement_free(HklStatement* stmt)
{

  assert(stmt != NULL);

  switch (stmt->type)
  {
    case HKL_STMT_PUTS:
    {
      // Free the expression
      hkl_expression_free(stmt->arg[0].expression);
    }
    break;

    default:
    break;
  }

  hkl_free_object(stmt);
}
