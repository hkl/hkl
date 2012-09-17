#ifndef HKL_PUTS_STMT_H
#define HKL_PUTS_STMT_H

#include "hkl_stmt.h"
#include "hkl_expression.h"

/**
@struct HklPutsStmt A container for hkl puts statements.

HklPutsStmt stores the expression component of a puts (std output) statement.

@authors Chris Knadler
@date 9/15/2012
*/

typedef struct HklPutsStmt
{
  HklExpression* expr;
} HklPutsStmt;

/**
Allocate a new HklPutsStmt

@param expr The expression in the puts stmt. Will eventually be evaluated as a string and printed.
@retval HklPutsStmt* A new HklPutsStmt.
*/
HklPutsStmt* hkl_puts_stmt_new(HklExpression* expr);

#endif
