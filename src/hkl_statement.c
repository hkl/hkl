#include <assert.h>
#include <stdio.h>

#include "hkl_alloc.h"
#include "hkl_statement.h"
#include "hklr.h"

HklStatement* hkl_statement_new(HklStatementType type, ...)
{
  assert(type != HKL_STMT_NONE);

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
      HklValue* value = hkl_expression_eval(stmt->arg[0].expression);  

      assert(value != NULL);
      
      switch (value->type)
      {
        case HKL_TYPE_INT:
          fprintf(stdout, "%i", value->as.integer);
          break;

        case HKL_TYPE_REAL:
          fprintf(stdout, "%lg", value->as.real);
          break;

        case HKL_TYPE_STRING:
          fprintf(stdout, "%s", value->as.string->utf8_data);
          break;

        default:
          break;
      }

      // flush the output
      fflush(stdout);

      hkl_value_free(value);
    }

    case HKL_STMT_HKLR:

      // Print runtime information
      fprintf(stdout, "Objects Created: %zu\n", HKLR.gc_created);
      fprintf(stdout, "Objects Freed:   %zu\n", HKLR.gc_freed);
      fprintf(stdout, "Object Cycles:   %zu\n", HKLR.gc_rootsize);
      fprintf(stdout, "GC Runs:         %zu\n", HKLR.gc_runs);
      fflush(stdout);
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
