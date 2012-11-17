#include <assert.h>

#include "hklr_expression.h"
#include "hklr.h"

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

  switch (value->type)
  {
    case HKL_TYPE_NIL:
      fprintf(stdout, "nil");
      break;

    case HKL_TYPE_INT:
      fprintf(stdout, "%i", value->as.integer);
      break;

    case HKL_TYPE_TYPE:
      switch (value->as.type)
      {
        case HKL_TYPE_NIL:
          fprintf(stdout, "nil");
        break;

        case HKL_TYPE_INT:
          fprintf(stdout, "int");
        break;

        case HKL_TYPE_TYPE:
          fprintf(stdout, "type");
        break;

        case HKL_TYPE_REAL:
          fprintf(stdout, "real");
        break;

        case HKL_TYPE_STRING:
          fprintf(stdout, "string");
        break;

        case HKL_TYPE_ARRAY:
          fprintf(stdout, "array");
        break;

        case HKL_TYPE_FUNCTION:
          fprintf(stdout, "func");
        break;

        default:
          assert(false);
        break;
      }
      break;

    case HKL_TYPE_REAL:
      fprintf(stdout, "%lg", value->as.real);
      break;

    case HKL_TYPE_STRING:
      fprintf(stdout, "%s", value->as.string->utf8_data);
      break;

    case HKL_TYPE_ARRAY:
    {
      HklDeque* deque = value->as.deque;
      size_t i;
      fprintf(stdout, "[");
      for (i = 0; i < value->as.deque->size - 1; ++i)
      {
        fprintf(stdout, "%i, ", ((HklValue*) hkl_deque_findn(deque, i))->as.integer);
      }
      if (i < value->as.deque->size)
      {
        fprintf(stdout, "%i", ((HklValue*) hkl_deque_findn(deque, i))->as.integer);
      }
      fprintf(stdout, "]");
    }
    break;

    default:
      assert(false);
      break;
  }

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
