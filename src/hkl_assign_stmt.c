#include <assert.h>

#include "hkl_assign_stmt.h"
#include "hkl_alloc.h"

HklAssignStmt* hkl_assign_stmt_new(HklVariable *var, HklExpression *expr, HklAssignOperator op)
{
  HklAssignStmt* assign_stmt = hkl_alloc_object(HklAssignStmt);
  assert(assign_stmt != NULL);

  assert(var != NULL);
  assert(expr != NULL);

  assign_stmt->var = var;
  assign_stmt->expr = expr;
  assign_stmt->op = op;

  return assign_stmt;
}

void hkl_assign_stmt_clear(HklAssignStmt* assign_stmt)
{
  assert(assign_stmt != NULL);

  hkl_variable_free(assign_stmt->var);
  assign_stmt->var = NULL;

  hkl_expression_free(assign_stmt->expr);
  assign_stmt->expr = NULL;

  assign_stmt->op = 0;
}

void hkl_assign_stmt_free(HklAssignStmt* assign_stmt)
{
  assert(assign_stmt != NULL);

  hkl_assign_stmt_clear(assign_stmt);

  free(assign_stmt);
}
