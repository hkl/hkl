#ifndef HKL_INCLUDE_STMT_H
#define HKL_INCLUDE_STMT_H

#include "hkl_stmt.h"
#include "hkl_expression.h"

/**
@struct HklIncludeStmt: a container for hkl include statements.

HklIncludeStmt stores the expression component of an include statement.

@authors Chris Knadler
@date 9/15/2012
*/

typedef struct HklIncludeStmt
{
  HklExpression* expr; 
} HklIncludeStmt;

/**
Allocate a new HklIncludeStmt

@param expr The expression in the include statement. This needs to evaluate to a string.
@retval HklIncludeStmt* A new HklIncludeStmt
*/
HklIncludeStmt* hkl_include_stmt_new(HklExpression* expr);

/**
Clear the contents of an HklIncludeStmt

@param include_stmt The HklIncludeStmt to clear.
*/
void hkl_include_stmt_clear(HklIncludeStmt* include_stmt);

/**
Free an HklIncludeStmt and clear its contents

@param include_stmt The HklIncludeStmt to free.
*/
void hkl_include_stmt_free(HklIncludeStmt* include_stmt);

#endif
