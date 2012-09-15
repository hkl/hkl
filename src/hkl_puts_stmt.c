#include <assert.h>

#include "hkl_puts_stmt.h"
#include "hkl_alloc.h"

HklPutsStmt* hkl_puts_stmt_new(HklExpression* expr)
{
  HklPutsStmt* puts_stmt = hkl_alloc_object(HklPutsStmt);
  assert(puts_stmt != NULL);

  assert(expr != NULL);

  puts_stmt->expr = expr;

  return puts_stmt;
}
