#ifndef HKL_ASSERT_STMT_H
#define HKL_ASSERT_STMT_H

#include "hkl_stmt.h"
#include "hkl_expression.h"

/**
@struct HklAssertStmt: a container for hkl asserts.

HklAssertStmt stores the expression component of an assert statement.

@authors Chris Knadler
@date 9/15/2012
*/

typedef struct HklAssertStmt : public HklStmt
{
  HklExpression* expr;
} HklAssertStmt;

/**
Allocate a new HklAssertStmt

@param expr The expression in the assert statement
@retval HklAssertStmt* A new HklAssertStmt
*/
HklAssertStmt* hkl_assert_stmt_new(HklExpression *expr);

/**
Get the expression of an HklAssertStmt

@param assert_stmt The HklAssertStmt object.
@retval HklExpression* The expression of the HklAssertStmt.
@brief Expression getter for HklAssertStmt.
*/
HklExpression* hkl_assert_stmt_get_expression(HklAssertStmt* assert_stmt);

#endif
