#ifndef HKL_RETURN_STMT_H
#define HKL_RETURN_STMT_H

#include "hkl_stmt.h"
#include "hkl_expression.h"

/**
@struct HklReturnStmt A container for hkl return statements

HklReturnStmt stores the expression to be evaluated and returned when the statement is called.

@authors Chris Knadler
@date 9/15/2012
*/
typedef struct HklReturnStmt
{
  struct HklStmt super;
  HklExpression* expr;
} HklReturnStmt;

/**
Allocate a new HklReturnStmt

@param expr The expression in the return statement.
@retval HklReturnStmt* A new HklReturnStmt
*/
HklReturnStmt* hkl_return_stmt_new(HklExpression* expr);
