#include <assert.h>

#include "hklr.h"
#include "hklr_expression.h"

void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs)
{
  // Evaluate the left hand side
  HklValue* vobj = hklr_expression_eval(lhs);
  HklrObject* object = vobj->as.object;
  hkl_value_free(vobj);

  hklr_object_assign(object, rhs);
}
