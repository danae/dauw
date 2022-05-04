#include "internals_value.h"

namespace dauw
{
  // Create a value containing nothing
  value_t value_nothing()
  {
    return VAL_NOTHING;
  }

  // Return if the value represents nothing
  bool value_is_nothing(value_t value)
  {
    return value == VAL_NOTHING;
  }

  // Create a value containing false
  value_t value_false()
  {
    return VAL_FALSE;
  }

  // Return if the value represents false
  bool value_is_false(value_t value)
  {
    return value == VAL_FALSE;
  }

  // Create a value containing true
  value_t value_true()
  {
    return VAL_TRUE;
  }

  // Return if the value represents true
  bool value_is_true(value_t value)
  {
    return value == VAL_TRUE;
  }

  // Convert a bool type to a value
  value_t value_of_bool(bool bool_value)
  {
    return bool_value ? VAL_TRUE : VAL_FALSE;
  }

  // Return if the value represents a bool type
  bool value_is_bool(value_t value)
  {
    return value == VAL_FALSE || value == VAL_TRUE;
  }

  // Convert a value to a bool type
  bool value_as_bool(value_t value)
  {
    if (!value_is_bool(value))
      throw std::domain_error("The value does not represent a valid bool type");

    return value == VAL_TRUE;
  }

  // Convert an int type to a value
  value_t value_of_int(int64_t int_value)
  {
    if (int_value & 0xffff'0000'0000'0000)
      int_value &= 0x0000'ffff'ffff'ffff;
    if (int_value & (1L << 48))
      throw std::out_of_range(fmt::format("The value {} exceeds the range of an int value because it exceeds 48 bits of storage", int_value));

    return (value_t)(BITMASK_QNAN | TAG_INT | ((value_t)int_value & BITMASK_VALUE));
  }

  // Return if the value represents an int type
  bool value_is_int(value_t value)
  {
    return (value & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_INT);
  }

  // Convert a value to an int type
  int64_t value_as_int(value_t value)
  {
    if (!value_is_int(value))
      throw std::domain_error("The value does not represent a valid int type");

    int64_t int_value = (int64_t)(value & BITMASK_VALUE);
    if (int_value & (1L << 47))
      return int_value | 0xffff'0000'0000'0000;
    else
      return int_value;
  }

  // Convert a rune type to a value
  value_t value_of_rune(rune_t rune_value)
  {
    if (rune_value > 0x10FFFF)
      throw std::out_of_range(fmt::format("The value U+{:06X} exceeds the range of a rune value because it exceeds the last code point U+10FFFF", rune_value));
    if (rune_value >= 0xD800 && rune_value <= 0xDFFF)
      throw std::out_of_range(fmt::format("The value U+{:06X} exceeds the range of a rune value because it contains a surrogate code point", rune_value));

    return (value_t)(BITMASK_QNAN | TAG_RUNE | ((value_t)rune_value & BITMASK_VALUE));
  }

  // Return if the value represents an rune type
  bool value_is_rune(value_t value)
  {
    return (value & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_RUNE);
  }

  // Convert a value to an rune type
  rune_t value_as_rune(value_t value)
  {
    if (!value_is_rune(value))
      throw std::domain_error("The value does not represent a valid rune type");

    rune_t rune_value = (rune_t)(value & BITMASK_VALUE);
    if (rune_value > 0x10FFFF)
      throw std::out_of_range("The value exceeds the range of a rune value because it exceeds the last code point U+10FFFF");
    if (rune_value >= 0xD800 && rune_value <= 0xDFFF)
      throw std::out_of_range("The value exceeds the range of a rune value because it contains a surrogate code point");

    return rune_value;
  }

  // Convert a pointer type to a value
  value_t value_of_ptr(uintptr_t ptr_value)
  {
    return (value_t)(BITMASK_QNAN | BITMASK_SIGN | (value_t)(uintptr_t)ptr_value & BITMASK_VALUE);
  }

  // Return if the value represents a pointer type
  bool value_is_ptr(value_t value)
  {
    return (value & (BITMASK_QNAN | BITMASK_SIGN)) == (BITMASK_QNAN | BITMASK_SIGN);
  }

  // Convert a value to a pointer type
  uintptr_t value_as_ptr(value_t value)
  {
    if (!value_is_ptr(value))
      throw std::domain_error("The value does not represent a valid pointer type");

    return (uintptr_t)(value & ~(BITMASK_QNAN | BITMASK_SIGN));
  }

  // Convert a real type to a value
  value_t value_of_real(double real_value)
  {
    value_t value;
    memcpy(&value, &real_value, sizeof(double));
    if ((value & BITMASK_QNAN) == BITMASK_QNAN)
      throw std::out_of_range("The value exceeds the range of a real value because it contains a quiet NaN");

    return value;
  }

  // Return if the value represents a real type
  bool value_is_real(value_t value)
  {
    return (value & BITMASK_QNAN) != BITMASK_QNAN;
  }

  // Convert a value to a real type
  double value_as_real(value_t value)
  {
    if (!value_is_real(value))
      throw std::domain_error("The value does not represent a valid real type");

    if (((value) & BITMASK_QNAN) == BITMASK_QNAN)
      throw std::out_of_range("The value exceeds the range of a real value because it contains a quiet NaN");

    double real_value;
    memcpy(&real_value, &value, sizeof(value_t));
    return real_value;
  }

  // Return if two values are equal
  bool value_equals(value_t lhs, value_t rhs)
  {
    if (value_is_real(lhs) && value_is_real(rhs))
      return value_as_real(lhs) == value_as_real(rhs);
    else
      return lhs == rhs;
  }

  // Formt a value
  string_t value_format(value_t value)
  {
    if (value_is_nothing(value))
      return fmt::format("nothing [{:x}]", value);
    else if (value_is_bool(value))
      return fmt::format("bool({}) [{:x}]", value_as_bool(value) ? "true" : "false", value);
    else if (dauw::value_is_int(value))
      return fmt::format("int({}) [{:x}]", value_as_int(value), value);
    else if (dauw::value_is_rune(value))
      return fmt::format("rune({}) [{:x}]", utf8::utf32to8(std::u32string(1, (char32_t)value_as_rune(value))), value);
    else if (dauw::value_is_ptr(value))
      return fmt::format("<pointer to address 0x{:016X}> [{:x}]", value_as_ptr(value), value);
    else if (dauw::value_is_real(value))
      return fmt::format("real({}) [{:x}]", value_as_real(value), value);
    else
      return fmt::format("{}", value);
  }


  // Convert a string to a pointer value
  value_t value_ptr_from_string(string_t string)
  {
    uintptr_t ptr_value = (uintptr_t)string.data();
    return value_of_ptr(ptr_value);
  }

  // Convert a pointer value to a string
  string_t value_uintptr_to_string(value_t value)
  {
    uintptr_t ptr_value = value_as_ptr(value);
    return string_t((const char*)ptr_value);
  }
}
