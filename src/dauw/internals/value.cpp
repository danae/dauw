#include "value.hpp"

namespace dauw
{
  // Constructor for a value
  Value::Value(value_t value, type_ptr type) : value_(value), type_(type)
  {
  }

  // Return the type of the value
  type_ptr& Value::type()
  {
    return type_;
  }

  // Return if the value represents nothing
  bool Value::is_nothing() const
  {
    return value_ == VAL_NOTHING;
  }

  // Return if the value represents false
  bool Value::is_false() const
  {
    return value_ == VAL_FALSE;
  }

  // Return if the value represents true
  bool Value::is_true() const
  {
    return value_ == VAL_TRUE;
  }

  // Convert a bool type to a value
  Value Value::of_bool(bool bool_value)
  {
    return Value(bool_value ? VAL_TRUE : VAL_FALSE, std::make_shared<Type>(TypeKind::BOOL));
  }

  // Return if the value represents a bool type
  bool Value::is_bool() const
  {
    return value_ == VAL_FALSE || value_ == VAL_TRUE;
  }

  // Convert a value to a bool type
  bool Value::as_bool() const
  {
    if (!is_bool())
      throw std::domain_error("The value does not represent a valid bool type");

    return value_ == VAL_TRUE;
  }

  // Convert an int type to a value
  Value Value::of_int(int64_t int_value)
  {
    if (int_value & INT_RANGE_CHECK)
    {
      if (int_value & INT_OF_NEG_CHECK)
        int_value &= INT_OF_NEG_MASK;
      else
        throw std::out_of_range(fmt::format("{} exceeds the valid range of an int; it exceeds 48 bits of storage", int_value));
    }

    return Value((value_t)(BITMASK_QNAN | TAG_INT | ((value_t)int_value & BITMASK_VALUE)), std::make_shared<Type>(TypeKind::INT));
  }

  // Return if the value represents an int type
  bool Value::is_int() const
  {
    return (value_ & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_INT);
  }

  // Convert a value to an int type
  int64_t Value::as_int() const
  {
    if (!is_int())
      throw std::domain_error("The value does not represent a valid int type");

    int64_t int_value = (int64_t)(value_ & BITMASK_VALUE);
    if (int_value & INT_AS_NEG_CHECK)
      return int_value | INT_AS_NEG_VAL;
    else
      return int_value;
  }

  // Convert a rune type to a value
  Value Value::of_rune(uint32_t rune_value)
  {
    if (rune_value > RUNE_MAX)
      throw std::out_of_range(fmt::format("U+{:06X} exceeds the valid range of a rune; it specifies a non-existing code point", rune_value));
    if (rune_value >= RUNE_SURROGATE_MIN && rune_value <= RUNE_SURROGATE_MAX)
      throw std::out_of_range(fmt::format("U+{:06X} exceeds the valid range of a rune; it specifies a surrogate code point", rune_value));

    return Value((value_t)(BITMASK_QNAN | TAG_RUNE | ((value_t)rune_value & BITMASK_VALUE)), std::make_shared<Type>(TypeKind::RUNE));
  }

  // Return if the value represents an rune type
  bool Value::is_rune() const
  {
    return (value_ & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_RUNE);
  }

  // Convert a value to an rune type
  uint32_t Value::as_rune() const
  {
    if (!is_rune())
      throw std::domain_error("The value does not represent a valid rune type");

    uint32_t rune_value = (uint32_t)(value_ & BITMASK_VALUE);
    if (rune_value > RUNE_MAX)
      throw std::out_of_range(fmt::format("U+{:06X} exceeds the valid range of a rune; it specifies a non-existing code point", rune_value));
    if (rune_value >= RUNE_SURROGATE_MIN && rune_value <= RUNE_SURROGATE_MAX)
      throw std::out_of_range(fmt::format("U+{:06X} exceeds the valid range of a rune; it specifies a surrogate code point", rune_value));

    return rune_value;
  }

  // Convert a real type to a value
  Value Value::of_real(double real_value)
  {
    value_t value;
    memcpy(&value, &real_value, sizeof(double));

    if ((value & BITMASK_QNAN) == BITMASK_QNAN)
      throw std::out_of_range("Value exceeds the valid range of a real; it contains a quiet NaN");

    return Value(value, std::make_shared<Type>(TypeKind::REAL));
  }

  // Return if the value represents a real type
  bool Value::is_real() const
  {
    return (value_ & BITMASK_QNAN) != BITMASK_QNAN;
  }

  // Convert a value to a real type
  double Value::as_real() const
  {
    if (!is_real())
      throw std::domain_error("The value does not represent a valid real type");

    if ((value_ & BITMASK_QNAN) == BITMASK_QNAN)
      throw std::out_of_range("Value exceeds the valid range of a real; it contains a quiet NaN");

    double real_value;
    memcpy(&real_value, &value_, sizeof(value_t));
    return real_value;
  }

  // Convert an object type to a value
  Value Value::of_obj(Obj* object_value, type_ptr object_type)
  {
    return Value((value_t)(BITMASK_QNAN | BITMASK_SIGN | (value_t)(uintptr_t)object_value & BITMASK_VALUE), object_type);
  }

  // Return if the value represents an object type
  bool Value::is_obj() const
  {
    return (value_ & (BITMASK_QNAN | BITMASK_SIGN)) == (BITMASK_QNAN | BITMASK_SIGN);
  }

  // Convert a value to a pointer type
  Obj* Value::as_obj() const
  {
    if (!is_obj())
      throw std::domain_error("The value does not represent a valid object type");

    return (Obj*)(uintptr_t)(value_ & ~(BITMASK_QNAN | BITMASK_SIGN));
  }

  // Assign another value to this value
  Value Value::operator=(const Value& other)
  {
    if (this != &other)
    {
      value_ = other.value_;
      type_ = other.type_;
    }
    return *this;
  }

  // Return if the value equals another value
  bool Value::operator==(const Value& other)
  {
    if (is_real() && other.is_real())
      return type_ == other.type_ && as_real() == other.as_real();
    else
      return type_ == other.type_ && value_ == other.value_;
  }
  bool Value::operator!=(const Value& other)
  {
    return !(*this == other);
  }

  // Return the truth value of the value
  bool Value::truth_value()
  {
    if (is_nothing())
      return false;
    else if (is_bool())
      return is_true();
    else if (is_int())
      return as_int() != 0;
    else if (is_rune())
      return as_rune() != 0;
    else if (is_real())
      return as_real() != 0;
    else if (is_obj())
      return true;
    else
      return false;
  }

  // Return a representative string representation of the value
  string_t Value::to_string()
  {
    if (is_nothing())
      return "nothing";
    else if (is_bool())
      return fmt::format("{}", as_bool() ? "true" : "false");
    else if (is_int())
      return fmt::format("{}", as_int());
    else if (is_rune())
      return fmt::format("'{}'", utf8::utf32to8(std::u32string(1, (uint32_t)as_rune())));
    else if (is_real())
      return fmt::format("{}", as_real());
    else if (is_obj())
      return as_obj()->to_string();
    else
      return fmt::format("<value {:#016x}>", value_);
  }
}
