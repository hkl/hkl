#ifndef HKL_ASSIGN_STMT_H
#define HKL_ASSIGN_STMT_H

#include "hkl_stmt.h"
#include "hkl_expression.h"
#include "hkl_variable.h"
#include "hkl_type.h"

/**
@struct HklAssignStmt: a container for hkl assign statments.

HklAssignStatement stores the variable, expression and operator of an assign statement. Only assign statement operators are accepted.

@authors Chris Knadler
@date 9/15/2012
*/

typedef struct HklAssignStmt : public HklStmt
{
  HklVariable* var;
  HklExpression* expr;
  HklOperator op;
} HklAssignStmt;

/**
Allocate a new HklAssignStmt

@param var The variable (left side) of the assign statment.
@param expr The expression (right side) of the assign statement.
@param op The operator (must be an assign operator).
@retval HklAssertStmt* The new HklAssertStmt.
*/
HklAssignStmt* hkl_assign_stmt_new(HklVariable *var, HklExpression *expr, HklOperator op);

#endif
