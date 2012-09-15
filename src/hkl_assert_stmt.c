#include <assert.h>

#include "hkl_assert_stmt.h"
#include "hkl_alloc.h"

HklAssertStmt* hkl_assert_stmt_new(HklExpression *expr)
{
  HklAssertStmt* assert_stmt = hkl_alloc_object(HklAssertStmt);
  assert(assert_stmt != NULL);

  assert(expr != NULL);
  assert_stmt->expr = expr;

  return assert_stmt;
}
