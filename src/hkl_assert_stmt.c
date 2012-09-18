#include <assert.h>

#include "hkl_assert_stmt.h"
#include "hkl_alloc.h"

HklAssertStmt* hkl_assert_stmt_new(HklExpression* expr)
{
  HklAssertStmt* assert_stmt = hkl_alloc_object(HklAssertStmt);
  assert(assert_stmt != NULL);

  assert(expr != NULL);
  assert_stmt->expr = expr;

  return assert_stmt;
}

void hkl_assert_stmt_clear(HklAssertStmt* assert_stmt)
{
  assert(assert_stmt != NULL);
  
  hkl_expression_free(assert_stmt->expr);
  
  assert_stmt->expr = NULL;
}

void hkl_assert_stmt_free(HklAssertStmt* assert_stmt)
{
  assert(assert_stmt != NULL);

  hkl_assert_stmt_clear(assert_stmt);

  free(assert_stmt);
}
