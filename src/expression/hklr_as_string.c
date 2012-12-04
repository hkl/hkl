#include <assert.h>

#include "hkl_value.h"
#include "hkl_string.h"

HklValue* hklr_as_string(HklValue* value);

static bool hash_build_string(HklPair* pair, void* string)
{
  HklValue* temp = hkl_value_new(HKL_TYPE_REF, pair->value);
  hkl_string_cat_utf8((HklString*) string, "\"");
  hkl_string_cat((HklString*) string, pair->key);
  hkl_string_cat_utf8((HklString*) string, "\"");
  hkl_string_cat_utf8((HklString*) string, ": ");
  hkl_string_cat((HklString*) string, hklr_as_string(temp)->as.string);
  hkl_string_cat_utf8((HklString*) string, ", ");

  return false;
}

// Make a new value object that is a cast of the value as a string
HklValue* hklr_as_string(HklValue* value)
{
  HklValue* cast = hkl_value_new(HKL_TYPE_STRING, NULL);

  switch (value->type)
  {
    case HKL_TYPE_REF:
    {
      HklValue* temp = hklr_object_dereference(value->as.object);
      cast->as.string = hklr_as_string(temp)->as.string;
    }
    break;

    case HKL_TYPE_NIL:
      cast->as.string = hkl_string_new_from_utf8("nil");
      break;

    case HKL_TYPE_INT:
      cast->as.string = hkl_string_new_from_integer(value->as.integer);
      break;

    case HKL_TYPE_TYPE:
      switch (value->as.type)
      {
        case HKL_TYPE_NIL:
          cast->as.string = hkl_string_new_from_utf8("nil");
        break;

        case HKL_TYPE_INT:
          cast->as.string = hkl_string_new_from_utf8("int");
        break;

        case HKL_TYPE_TYPE:
          cast->as.string = hkl_string_new_from_utf8("type");
        break;

        case HKL_TYPE_REAL:
          cast->as.string = hkl_string_new_from_utf8("real");
        break;

        case HKL_TYPE_STRING:
          cast->as.string = hkl_string_new_from_utf8("string");
        break;

        case HKL_TYPE_ARRAY:
          cast->as.string = hkl_string_new_from_utf8("array");
        break;

        case HKL_TYPE_HASH:
          cast->as.string = hkl_string_new_from_utf8("hash");
        break;

        case HKL_TYPE_FUNCTION:
          cast->as.string = hkl_string_new_from_utf8("func");
        break;

        case HKL_TYPE_CFUNC:
          cast->as.string = hkl_string_new_from_utf8("cfunc");
        break;

        default:
          assert(false);
        break;
      }
      break;

    case HKL_TYPE_REAL:
      cast->as.string = hkl_string_new_from_real(value->as.real);
      break;

    case HKL_TYPE_STRING:
      cast->as.string = hkl_string_new_from_string(value->as.string);
      break;

    case HKL_TYPE_ARRAY:
    {
      cast->as.string = hkl_string_new_from_utf8("[");

      HklDeque* deque = value->as.deque;
      size_t i;
      for (i = 0; i < value->as.deque->size; ++i)
      {
        HklValue* temp = hklr_as_string((HklValue*) hkl_deque_findn(deque, i));
        hkl_string_cat(cast->as.string, temp->as.string);
        if (i != value->as.deque->size - 1)
          hkl_string_cat_utf8(cast->as.string, ", ");

        hkl_value_free(temp);
      }

      hkl_string_cat_utf8(cast->as.string, "]");
    }
    break;

    case HKL_TYPE_HASH:
    {
      cast->as.string = hkl_string_new_from_utf8("{");

      HklHash* hash = value->as.hash;

      hkl_hash_traverse(hash, hash_build_string, cast->as.string);

      hkl_string_cat_utf8(cast->as.string, "}");
    }
    break;

    default:
      assert(false);
      break;
  }

  return cast;
}