#pragma once

#include "common.h"
#include "errors.h"

#include <memory>
#include <stdexcept>

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

// Defines for values of constaant types
#define VAL_NOTHING         ((value_t)(BITMASK_QNAN | TAG_NOTHING))
#define VAL_FALSE           ((value_t)(BITMASK_QNAN | TAG_FALSE))
#define VAL_TRUE            ((value_t)(BITMASK_QNAN | TAG_TRUE))


namespace dauw
{
  // Value that represents a nothing type
  value_t value_nothing();
  bool value_is_nothing(value_t value);

  // Value tha trepresents a bool type
  value_t value_false();
  bool value_is_false(value_t value);

  value_t value_true();
  bool value_is_true(value_t value);

  value_t value_of_bool(bool bool_value);
  bool value_is_bool(value_t value);
  bool value_as_bool(value_t value);

  // Value that represents an int type
  value_t value_of_int(int64_t int_value);
  bool value_is_int(value_t value);
  int64_t value_as_int(value_t value);

  // Value that represents a rune type
  value_t value_of_rune(char32_t rune_value);
  bool value_is_rune(value_t value);
  char32_t value_as_rune(value_t value);

  // Value that represents a pointer type
  value_t value_of_ptr(uintptr_t ptr_value);
  bool value_is_ptr(value_t value);
  uintptr_t value_as_ptr(value_t value);

  // Value that represents a real type
  value_t value_of_real(double real_value);
  bool value_is_real(value_t value);
  double value_as_real(value_t value);

  // Return if two values are equal
  bool value_equals(value_t lhs, value_t rhs);

  // Formt a value
  string_t value_format(value_t value);


  // Convert a string to a pointer value
  value_t value_ptr_from_string(string_t string);

  // Convert a pointer value to a string
  string_t value_uintptr_to_string(value_t value);

  // Convert a shared pointer to a pointer value
  template <typename T>
  inline value_t value_ptr_from_shared_ptr(std::shared_ptr<T> value)
  {
    uintptr_t ptr_value = (uintptr_t)value.get();
    return value_of_ptr(ptr_value);
  }

  // Convert a pointer value to a shared pointer
  template <typename T>
  inline std::shared_ptr<T> value_uintptr_to_shared_ptr(value_t value)
  {
    uintptr_t ptr_value = value_as_ptr(value);
    return std::shared_ptr<T>(ptr_value);
  }
}
