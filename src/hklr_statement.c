#include <assert.h>
#include <stdio.h>

#include "hkl_alloc.h"
#include "hklr_statement.h"
#include "hklr.h"

HklrStatement* hklr_statement_new(HklStatementType type, ...)
{
  assert(type != HKL_STMT_NONE);

  HklrStatement* stmt = hkl_alloc_object(HklrStatement);
  stmt->type = type;

  va_list argp;
  va_start(argp, type);

  switch (type)
  {
    case HKL_STMT_PUTS:
      // puts requires 1 expression
      stmt->arg[0].expression = va_arg(argp, HklrExpression*);
      break;

    case HKL_STMT_ASSIGN:

      stmt->arg[0].expression = va_arg(argp, HklrExpression*);
      stmt->arg[1].expression = va_arg(argp, HklrExpression*);
      break;

    default:
      break;
  }

  va_end(argp);

  return stmt;
}

void hklr_statement_exec(HklrStatement* stmt)
{
  assert(stmt != NULL);

  switch (stmt->type)
  {

    case HKL_STMT_PUTS:
    {
      HklValue* value = hklr_expression_eval(stmt->arg[0].expression);  

      assert(value != NULL);
      
      switch (value->type)
      {
        case HKL_TYPE_INT:
          fprintf(stdout, "%i", value->as.integer);
          break;

        case HKL_TYPE_REAL:
          fprintf(stdout, "%lg", value->as.real);
          break;

        case HKL_TYPE_STRING:
          fprintf(stdout, "%s", value->as.string->utf8_data);
          break;

        case HKL_TYPE_REF:
        {
          HklrObject* object = value->as.object;
          assert(object != NULL);

          switch(object->type)
          {
            case HKL_TYPE_INT:
              fprintf(stdout, "%i", object->as.integer);
            break;
            case HKL_TYPE_REAL:
              fprintf(stdout, "%lg", object->as.real);
            break;
            case HKL_TYPE_STRING:
              fprintf(stdout, "%s", object->as.string->utf8_data);
            break;
          }
        }
        break; // HKL_TYPE_REF

        default:
          assert(false);
          break;
      }

      // flush the output
      fflush(stdout);

      hkl_value_free(value);
    }
    break;

    case HKL_STMT_HKLR:

      // Print runtime information
      fprintf(stdout, "Ops:             %zu\n", HKLR.ops);
      fprintf(stdout, "Objects Created: %zu\n", HKLR.gc_created);
      fprintf(stdout, "Objects Freed:   %zu\n", HKLR.gc_freed);
      fprintf(stdout, "Object Cycles:   %zu\n", HKLR.gc_rootsize);
      fprintf(stdout, "GC Runs:         %zu\n", HKLR.gc_runs);
      fprintf(stdout, "Scope Level:     %zu\n", HKLR.scope_level);
      fprintf(stdout, "Globals:         %zu\n", HKLR.globals->length);
      fprintf(stdout, "Locals:          %zu\n", HKLR.scopes->locals->length);
      fprintf(stdout, "Upvals:          %zu\n", HKLR.scopes->upvals->length);
      fflush(stdout);
      break;

    case HKL_STMT_ASSIGN:
    {

      // Evaluate the left hand side and then discard the value object
      HklValue* vobj = hklr_expression_eval(stmt->arg[0].expression);
      HklrObject* object = vobj->as.object;
      hkl_value_free(vobj);

      HklValue* value = hklr_expression_eval(stmt->arg[1].expression);

      assert(object != NULL);
      assert(value != NULL);

      // wipe out the original value
      // if need be
      switch (object->type)
      {
        case HKL_TYPE_STRING:
          hkl_string_free(object->as.string);
        break;

        default:
        break;
      }

      switch (value->type)
      {
        case HKL_TYPE_INT:
          object->type = HKL_TYPE_INT;
          object->as.integer = value->as.integer;
        break;

        case HKL_TYPE_REAL:
          object->type = HKL_TYPE_REAL;
          object->as.real = value->as.real;
        break;

        case HKL_TYPE_STRING:
          object->type = HKL_TYPE_STRING;
          object->as.string = hkl_string_new_from_string(value->as.string);
        break;

        case HKL_TYPE_REF:
        {
          HklrObject* ref = value->as.object;
          assert(ref != NULL);

          switch(ref->type)
          {
            case HKL_TYPE_INT:
              object->type = HKL_TYPE_INT;
              object->as.integer = ref->as.integer;
            break;

            case HKL_TYPE_REAL:
              object->type = HKL_TYPE_REAL;
              object->as.real = ref->as.real;
            break;

            case HKL_TYPE_STRING:
              object->type = HKL_TYPE_STRING;
              object->as.string = hkl_string_new_from_string(ref->as.string);
            break;

            default:
            break;
          }
        }
        break; // HKL_TYPE_REF
      }

      hkl_value_free(value);
    }
    break; // HKL_STMT_ASSIGN

    default:
      break;
  }

  // Increment numbers of completed operations
  HKLR.ops++;
}

void hklr_statement_free(HklrStatement* stmt)
{

  assert(stmt != NULL);

  switch (stmt->type)
  {
    case HKL_STMT_PUTS:
      // Free the expression
      hklr_expression_free(stmt->arg[0].expression);
      break;

    case HKL_STMT_ASSIGN:
      hklr_expression_free(stmt->arg[0].expression);
      hklr_expression_free(stmt->arg[1].expression);
      break;

    default:
    break;
  }

  hkl_free_object(stmt);
}
