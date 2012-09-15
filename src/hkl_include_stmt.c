#include <assert.h>

#include "hkl_include_stmt.h"
#include "hkl_alloc.h"

HklIncludeStmt* hkl_include_stmt_new(HklExpression* expr)
{
  HklIncludeStmt* include_stmt = hkl_alloc_object(HklIncludeStmt);
  assert(include_stmt != NULL);

  assert(expr != NULL);

  include_stmt->expr = expr;

  return include_stmt;
}
