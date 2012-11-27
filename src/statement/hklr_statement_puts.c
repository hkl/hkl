#include <assert.h>

#include "hklr_expression.h"
#include "hklr.h"

extern HklValue* hklr_as_string(HklValue* value);

void hklr_statement_puts(HklrExpression* expr)
{
  assert(expr != NULL);

  HklValue* value = hklr_expression_eval(expr);

  bool temporary = true;

  // derefence the object
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    
    // Don't free the deque or hash since it can't be a temporary
    if (value->type == HKL_TYPE_ARRAY)
    {
      temporary = false;
      // simply spoof the value
      temp->type = HKL_TYPE_NIL;
    }
    
    hkl_value_free(temp);
  }

  HklValue* cast = hklr_as_string(value);
  printf("%s", cast->as.string->utf8_data);
  hkl_value_free(cast);

  // flush the output
  fprintf(stdout, "\n");
  fflush(stdout);
  
  // Be sure not to delete the deque if its not a temporary
  if (value->type == HKL_TYPE_ARRAY && temporary == false)
  {
    value->type = HKL_TYPE_NIL;
  }
  
  hkl_value_free(value);
}
