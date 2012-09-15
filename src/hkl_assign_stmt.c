#include <assert.h>

#include "hkl_assign_stmt.h"
#include "hkl_alloc.h"

HklAssignStmt* hkl_assign_stmt_new(HklVariable *var, HklExpression *expr, HklAssignOperator op)
{
  HklAssignStmt* assign_stmt = hkl_alloc_object(HklAssignStmt);
  assert(assign_stmt != NULL);

  assert(var != NULL);
  assert(expr != NULL);
  assert(op != NULL);

  assign_stmt->var = var;
  assign_stmt->expr = expr;
  assign_stmt->op = op;

  return assign_stmt;
}
