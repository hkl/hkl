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

typedef struct HklAssignStmt
{
  HklVariable* var;
  HklExpression* expr;
  HklAssignOperator op;
} HklAssignStmt;

/**
Allocate a new HklAssignStmt

@param var The variable (left side) of the assign statment.
@param expr The expression (right side) of the assign statement.
@param op The type of assignment operator used in the assignment statment.
@retval HklAssertStmt* The new HklAssertStmt.
*/
HklAssignStmt* hkl_assign_stmt_new(HklVariable *var, HklExpression *expr, HklAssignOperator op);

/**
Erase the contents of an HklAssignStmt

@post The HklVariable and HklExpression associated are freed in the process.

@param assign_stmt The HklAssignStmt to clear.
*/
void hkl_assign_stmt_clear(HklAssignStmt* assign_stmt);

/**
Free an HklAssignStmt and relase any allocated resources it has.

@param assign_stmt The HklAssignStmt to free.
*/
void hkl_assign_stmt_free(HklAssignStmt* assign_stmt);

#endif
