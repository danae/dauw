#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/internals/object.hpp>


// Type definition for the value type
using value_t = uint64_t;

// Defines for the IEEE 754 bitmasks
#define BITMASK_SIGN        ((value_t)0x8000'0000'0000'0000)
#define BITMASK_QNAN        ((value_t)0x7ff8'0000'0000'0000)
#define BITMASK_TAG         ((value_t)0x0007'0000'0000'0000)
#define BITMASK_SIGNATURE   ((value_t)0xffff'0000'0000'0000)
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
#define INT_NEGATIVE        ((int64_t)0x0000'8000'0000'0000)
#define RUNE_MAX            ((uint32_t)0x10ffff)
#define RUNE_SURROGATE_MIN  ((uint32_t)0x00d800)
#define RUNE_SURROGATE_MAX  ((uint32_t)0x00dfff)


namespace dauw::internals
{
  // Class that represents a value in stack memory
  class Value
  {
    private:
      // The actual value
      value_t value_;


    public:
      // Constructor
      Value(value_t value);

      // Value that represents a constant type
      bool is_nothing() const;
      void as_nothing() const;

      // Value tha trepresents a bool type
      static Value of_bool(dauw_bool_t bool_value);
      bool is_bool() const;
      bool is_false() const;
      bool is_true() const;
      dauw_bool_t as_bool() const;

      // Value that represents an int type
      static Value of_int(dauw_int_t int_value);
      bool is_int() const;
      dauw_int_t as_int() const;

      // Value that represents a rune type
      static Value of_rune(dauw_rune_t rune_value);
      bool is_rune() const;
      dauw_rune_t as_rune() const;

      // Value that represents a real type
      static Value of_real(dauw_real_t real_value);
      bool is_real() const;
      dauw_real_t as_real() const;

      // Value that represents an object type
      static Value of_obj(Obj* object_value, Type object_type);
      bool is_obj() const;
      Obj* as_obj() const;

      // Return the type of the value
      Type& type();

      // Assign another value to this value
      Value operator=(const Value& other);

      // Return if the value equals another value
      bool operator==(const Value& other);
      bool operator!=(const Value& other);

      // Return the truth value of the value
      bool truth_value();

      // Return a representative string representation of the value
      string_t to_string();


      // Definitions for global values
      static Value value_nothing;
      static Value value_false;
      static Value value_true;
  };


  // Exception thrown when processing a value fails
  class ValueException : public Exception
  {
    public:
      inline ValueException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline ValueException(string_t message) : Exception(message, nullptr) {}
  };

  // Exception thrown when a value is not of the expected value type
  class ValueMismatchException : public ValueException
  {
    public:
      inline ValueMismatchException(string_t message, Exception* previous) : ValueException(message, previous) {}
      inline ValueMismatchException(string_t message) : ValueException(message, nullptr) {}
  };

  // Exception thrown when a value exceeds the range of the expected value type
  class ValueOverflowException : public ValueException
  {
    public:
      inline ValueOverflowException(string_t message, Exception* previous) : ValueException(message, previous) {}
      inline ValueOverflowException(string_t message) : ValueException(message, nullptr) {}
  };
}


namespace fmt
{
  using namespace dauw;
  using namespace dauw::internals;

  // Class that defines a formatter for a value
  template <>
  struct formatter<Value> : formatter<string_view_t>
  {
    inline string_t stringify(Value value)
    {
      if (value.is_nothing())
        return "nothing";
      else if (value.is_bool())
        return fmt::format("{}", value.as_bool() ? "true" : "false");
      else if (value.is_int())
        return fmt::format("{}", value.as_int());
      else if (value.is_rune())
        return fmt::format("'{}'", dauw::utils::string::rune_pack_to_str(value.as_rune()));
      else if (value.is_real())
        return fmt::format("{:#}", value.as_real());
      else if (value.is_obj())
        return fmt::format("{}", *value.as_obj());
      else
        return fmt::format("<invalid value>");
    }

    template <typename FormatContext>
    auto format(Value value, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(value), ctx);
    }
  };
}
