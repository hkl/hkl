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

typedef struct HklAssertStmt
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
Erase the contents of a HklAssertStmt

@post The HklExpression associated is freed in the process.

@param assert_stmt The HklAssertStmt to clear.
*/
void hkl_assert_stmt_clear(HklAssertStmt* assert_stmt);

/**
Free an HklAssertStmt and release any allocated resources it has.

@param assert_stmt The HklAssertStmt to free.
*/
void hkl_assert_stmt_free(HklAssertStmt* assert_stmt);

#endif
