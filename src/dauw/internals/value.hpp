#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/utils/string.hpp>


// Type definition for the value type
using value_t = uint64_t;

// Defines for the IEEE 754 bitmasks
#define BITMASK_SIGN        ((value_t)0x8000'0000'0000'0000)
#define BITMASK_QNAN        ((value_t)0x7ff8'0000'0000'0000)
#define BITMASK_TAG         ((value_t)0x0007'0000'0000'0000)
#define BITMASK_VALUE       ((value_t)0x0000'ffff'ffff'ffff)

// Defines for tags of primitives
#define TAG_CONST           ((value_t)0x0000'0000'0000'0000)
#define TAG_INT             ((value_t)0x0001'0000'0000'0000)

// Defines for constants of primitives
#define CONST_NOTHING       ((value_t)0x0000'0000'0000'0001)
#define CONST_FALSE         ((value_t)0x0000'0000'0000'0002)
#define CONST_TRUE          ((value_t)0x0000'0000'0000'0003)

// Defines for values of primitives
#define VAL_NOTHING         ((value_t)(BITMASK_QNAN | TAG_CONST | CONST_NOTHING))
#define VAL_FALSE           ((value_t)(BITMASK_QNAN | TAG_CONST | CONST_FALSE))
#define VAL_TRUE            ((value_t)(BITMASK_QNAN | TAG_CONST | CONST_TRUE))

#define VAL_INF_POSITIVE    ((value_t)0x7ff0'0000'0000'0000)
#define VAL_INF_NEGATIVE    ((value_t)0xfff0'0000'0000'0000)
#define VAL_NAN             ((value_t)0x7ff0'ffff'ffff'ffff)


namespace dauw
{
  // Class that represents a value in stack memory
  class Value
  {
    private:
      // The underlying value of the value
      value_t value_;


    public:
      // Constructor
      Value(value_t value);

      // Value that represents a constant type
      static Value of_nothing();
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

      // Value that represents a float type
      static Value of_float(dauw_float_t float_value);
      bool is_float() const;
      bool is_nan() const;
      dauw_float_t as_float() const;

      // Value that represents an object type
      static Value of_obj(Obj* object_value);
      bool is_obj() const;
      Obj* as_obj() const;

      // Return the type of the value
      Type& type();

      // Return if the value equals another value
      bool operator==(const Value& other);
      bool operator!=(const Value& other);


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
