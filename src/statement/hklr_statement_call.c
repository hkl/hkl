#include <assert.h>

#include "hklr_expression.h"
#include "hklr_statement.h"
#include "hkl_list.h"
#include "hklr.h"

extern void hklr_statement_assign(HklrExpression* lhs, HklrExpression* rhs);

static void hklr_statement_exec_list(void* stmt, void* data)
{
  hklr_statement_exec((HklrStatement*) stmt);
}

static void make_locals(void* string, void* args_head)
{
  HklrObject* object = hklr_object_new(HKL_TYPE_NIL, HKL_FLAG_NONE);

  hklr_local_insert((HklString*) string, object);

  // If you have too few args then the rest are nil
  if ((*(HklListNode**) args_head) == NULL)
    return;

  // Some fancy pointer arithematic
  // This iterates the argument expression
  HklrExpression* assign = (*((HklListNode**) args_head))->data;
  *((HklListNode**) args_head) = (*((HklListNode**) args_head))->next;

  // This is a hack for now since hklr_statement_assign takes 2 expressions
  // This just makes a fake one
  HklrExpression* expr = hklr_expression_new(HKL_EXPR_VAR, hkl_string_new_from_string((HklString*) string), hkl_list_new());
  hklr_statement_assign(expr, assign);
  hklr_expression_free(expr);
}

void hklr_statement_call(HklrExpression* expr, HklList* args)
{
  assert(expr != NULL);
  assert(args != NULL);

  HklValue* value = hklr_expression_eval(expr);
  assert(value != NULL);

  // dereference the objcet
  if (value->type == HKL_TYPE_REF)
  {
    HklValue* temp = value;
    value = hklr_object_dereference(value->as.object);
    hkl_value_free(temp);
  }

  // The calling object should be a function
  assert(value->type == HKL_TYPE_FUNCTION);

  HklrFunction* function = value->as.function;
  hkl_value_free(value);

  hklr_scope_push();

  // Make the args in the function signature local variables

  HklListNode* args_head = args->head; // This is an iterator for the args
  hkl_list_traverse(function->args_list, make_locals, &args_head);
  
  // execute the statements within
  hkl_list_traverse(function->stmt_list, hklr_statement_exec_list, NULL);

  hklr_scope_pop();
}