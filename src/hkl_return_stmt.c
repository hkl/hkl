#include <assert.h>

#include "hkl_return_stmt.h"
#include "hkl_alloc.h"

HklReturnStmt* hkl_return_stmt_new(HklExpression* expr)
{
  HklReturnStmt* return_stmt = hkl_alloc_object(HklReturnStmt);
  assert(return_stmt != NULL);

  assert(expr != NULL);

  return_stmt->expr = expr;

  return return_stmt;
}

void hkl_return_stmt_clear(HklReturnStmt* return_stmt)
{
  assert(return_stmt != NULL);

  hkl_expression_free(return_stmt->expr);

  return_stmt->expr = NULL;
}

void hkl_return_stmt_free(HklReturnStmt* return_stmt)
{
  assert(return_stmt != NULL);

  hkl_return_stmt_clear(return_stmt);

  free(return_stmt);
}
