#include <assert.h>
#include <stdio.h>

#include "hkl_alloc.h"
#include "hklr_statement.h"
#include "hklr.h"

extern void hklr_statement_puts(HklrExpression* expr);
extern void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs);
extern void hklr_statement_if(HklrExpression* expr, HklList* list);
extern void hklr_statement_while(HklrExpression* expr, HklList* list);
extern void hklr_statement_call(HklrExpression* expr, HklList* list);

HklrStatement* hklr_statement_new(HklStatementType type, ...)
{
  assert(type != HKL_STMT_NONE);

  HklrStatement* stmt = hkl_alloc_object(HklrStatement);
  stmt->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_STMT_PUTS:
      // puts requires 1 expression
      stmt->arg[0].expression = va_arg(argp, HklrExpression*);
      break;

    case HKL_STMT_ASSIGN:

      stmt->arg[0].expression = va_arg(argp, HklrExpression*);
      stmt->arg[1].expression = va_arg(argp, HklrExpression*);
      break;

    case HKL_STMT_IF:
    case HKL_STMT_WHILE:
    case HKL_STMT_CALL:

      stmt->arg[0].expression = va_arg(argp, HklrExpression*);
      stmt->arg[1].list = va_arg(argp, HklList*);
      break;

    default:
      break;
  }

  va_end(argp);

  return stmt;
}

void hklr_statement_exec(HklrStatement* stmt)
{
  assert(stmt != NULL);

  switch (stmt->type)
  {

    case HKL_STMT_PUTS:
      hklr_statement_puts(stmt->arg[0].expression);
    break;

    case HKL_STMT_HKLR:

      // Print runtime information
      fprintf(stdout, "Ops:             %zu\n", HKLR.ops);
      fprintf(stdout, "Objects Created: %zu\n", HKLR.gc_created);
      fprintf(stdout, "Objects Freed:   %zu\n", HKLR.gc_freed);
      fprintf(stdout, "Objects Alive:   %zu\n", HKLR.gc_created - HKLR.gc_freed);
      fprintf(stdout, "Bacon:           %zu\n", HKLR.gc_rootsize);
      fprintf(stdout, "GC Runs:         %zu\n", HKLR.gc_runs);
      fprintf(stdout, "Globals:         %zu\n", HKLR.globals->length);
      fprintf(stdout, "Locals:          %zu\n", ((HklScope*) HKLR.scopes->tail->data)->locals->length);
      fprintf(stdout, "Upvals:          %zu\n", ((HklScope*) HKLR.scopes->tail->data)->upvals->length);
      fflush(stdout);
      break;

    case HKL_STMT_COLLECT:
      hklr_gc_collect();
    break;

    case HKL_STMT_ASSIGN:
      hklr_statement_assign(stmt->arg[0].expression, stmt->arg[1].expression);
    break;

    case HKL_STMT_IF:
      hklr_statement_if(stmt->arg[0].expression, stmt->arg[1].list);
      break;

    case HKL_STMT_WHILE:
      hklr_statement_while(stmt->arg[0].expression, stmt->arg[1].list);
      break;

    case HKL_STMT_CALL:
      hklr_statement_call(stmt->arg[0].expression, stmt->arg[1].list);
    break;

    default:
      break;
  }

  // Increment numbers of completed operations
  HKLR.ops++;
}

// helper function to free all items in a statement list
static void hklr_statement_free_list(void* stmt, void* data) {

  hklr_statement_free((HklrStatement*) stmt);
}

static void hklr_expression_free_list(void* expr, void* data) {

  hklr_expression_free((HklrExpression*) expr);
}

void hklr_statement_free(HklrStatement* stmt)
{
  assert(stmt != NULL);

  switch (stmt->type)
  {
    case HKL_STMT_PUTS:
      // Free the expression
      hklr_expression_free(stmt->arg[0].expression);
      break;

    case HKL_STMT_ASSIGN:
      hklr_expression_free(stmt->arg[0].expression);
      hklr_expression_free(stmt->arg[1].expression);
      break;

    case HKL_STMT_IF:
    case HKL_STMT_WHILE:
      hklr_expression_free(stmt->arg[0].expression);
      hkl_list_traverse(stmt->arg[1].list, hklr_statement_free_list, NULL);
      hkl_list_free(stmt->arg[1].list);
      break;

    case HKL_STMT_CALL:
      hklr_expression_free(stmt->arg[0].expression);
      hkl_list_traverse(stmt->arg[1].list, hklr_expression_free_list, NULL);
      hkl_list_free(stmt->arg[1].list);
      break;

    default:
    break;
  }

  hkl_free_object(stmt);
}
