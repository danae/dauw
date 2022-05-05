#pragma once

#include "common.h"
#include "errors.h"
#include "internals_object.h"

#include <memory>


// Type definition for the value type
using value_t = uint64_t;

// Defines for the IEEE 754 bitmasks
#define BITMASK_SIGN        ((value_t)0x8000'0000'0000'0000)
#define BITMASK_QNAN        ((value_t)0x7ff8'0000'0000'0000)
#define BITMASK_TAG         ((value_t)0x0007'0000'0000'0000)
#define BITMASK_VALUE       ((value_t)0x0000'ffff'ffff'ffff)

// Defines for tags of primitives
#define TAG_NOTHING         ((value_t)0x0001'0000'0000'0000)
#define TAG_FALSE           ((value_t)0x0002'0000'0000'0000)
#define TAG_TRUE            ((value_t)0x0003'0000'0000'0000)
#define TAG_INT             ((value_t)0x0004'0000'0000'0000)
#define TAG_RUNE            ((value_t)0x0005'0000'0000'0000)
#define TAG_UNUSED1         ((value_t)0x0006'0000'0000'0000)
#define TAG_UNUSED2         ((value_t)0x0007'0000'0000'0000)

// Defines for values of constant types
#define VAL_NOTHING         ((value_t)(BITMASK_QNAN | TAG_NOTHING))
#define VAL_FALSE           ((value_t)(BITMASK_QNAN | TAG_FALSE))
#define VAL_TRUE            ((value_t)(BITMASK_QNAN | TAG_TRUE))

// Defines for allowed value ranges
#define INT_RANGE_CHECK     ((int64_t)0xffff'0000'0000'0000)
#define INT_OF_NEG_CHECK    ((int64_t)0xffff'8000'0000'0000)
#define INT_OF_NEG_MASK     ((int64_t)0x0000'ffff'ffff'ffff)
#define INT_AS_NEG_CHECK    ((int64_t)0x0000'8000'0000'0000)
#define INT_AS_NEG_VAL      ((int64_t)0xffff'0000'0000'0000)

#define RUNE_MAX            ((uint32_t)0x10ffff)
#define RUNE_SURROGATE_MIN  ((uint32_t)0x00d800)
#define RUNE_SURROGATE_MAX  ((uint32_t)0x00dfff)


namespace dauw
{
  // Class that represents a value in stack memory
  class Value
  {
    private:
      // The actual value
      value_t value_;


    public:
      // Constants for constant values
      static const Value value_nothing;
      static const Value value_false;
      static const Value value_true;


      // Constructor
      Value(value_t value);

      // Value that represents a constant type
      bool is_nothing() const;
      bool is_false() const;
      bool is_true() const;

      // Value tha trepresents a bool type
      static Value of_bool(bool bool_value);
      bool is_bool() const;
      bool as_bool() const;

      // Value that represents an int type
      static Value of_int(int64_t int_value);
      bool is_int() const;
      int64_t as_int() const;

      // Value that represents a rune type
      static Value of_rune(uint32_t rune_value);
      bool is_rune() const;
      uint32_t as_rune() const;

      // Value that represents a real type
      static Value of_real(double real_value);
      bool is_real() const;
      double as_real() const;

      // Value that represents an object type
      static Value of_obj(Obj* object_value);
      bool is_obj() const;
      Obj* as_obj() const;

      // Assign another value to this value
      Value operator=(const Value& other);

      // Return if the value equals another value
      bool operator==(const Value& other);
      bool operator!=(const Value& other);

      // Return a representative string representation of the value
      string_t to_string();
  };
}


namespace fmt
{
  // Class that defines a formatter for a value
  template <>
  struct formatter<dauw::Value> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(dauw::Value value, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(value.to_string(), ctx);
    }
  };
}
