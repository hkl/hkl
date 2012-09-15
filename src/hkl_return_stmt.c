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
