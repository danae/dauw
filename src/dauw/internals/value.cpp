#include "value.hpp"

namespace dauw::internals
{
  // Initialize the definitions for global values
  Value Value::value_nothing = Value(VAL_NOTHING);
  Value Value::value_false = Value(VAL_FALSE);
  Value Value::value_true = Value(VAL_TRUE);


  // Constructor for a value
  Value::Value(value_t value)
    : value_(value)
  {
  }

  // Constructor for a value from another value
  Value::Value(const Value& other)
    : value_(other.value_)
  {
  }

  // Assignment from a value from an underlying value
  Value Value::operator=(value_t value)
  {
    value_ = value;
    return *this;
  }

  // Assignment from a value from another value
  Value Value::operator=(const Value& other)
  {
    value_ = other.value_;
    return *this;
  }

  // Return if the value represents nothing
  bool Value::is_nothing() const
  {
    return value_ == VAL_NOTHING;
  }

  // Convert a value to a nothing type
  void Value::as_nothing() const
  {
    if (!is_nothing())
      throw ValueMismatchException(fmt::format("The value {:#016x} does not represent a valid nothing value", value_));
  }

  // Convert a bool type to a value
  Value Value::of_bool(bool bool_value)
  {
    return Value(bool_value ? VAL_TRUE : VAL_FALSE);
  }

  // Return if the value represents a bool type
  bool Value::is_bool() const
  {
    return value_ == VAL_FALSE || value_ == VAL_TRUE;
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

  // Convert a value to a bool type
  bool Value::as_bool() const
  {
    if (!is_bool())
      throw ValueMismatchException(fmt::format("The value {:#016x} does not represent a valid bool value", value_));

    return value_ == VAL_TRUE;
  }

  // Convert an int type to a value
  Value Value::of_int(dauw_int_t int_value)
  {
    if (int_value & BITMASK_SIGNATURE)
    {
      if (int_value & (BITMASK_SIGNATURE | INT_NEGATIVE))
        int_value &= BITMASK_VALUE;
      else
        throw ValueOverflowException(fmt::format("The int {} exceeds the available int value storage of 48 bits", int_value));
    }

    return Value((value_t)(BITMASK_QNAN | TAG_INT | ((value_t)int_value & BITMASK_VALUE)));
  }

  // Return if the value represents an int type
  bool Value::is_int() const
  {
    return (value_ & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_INT);
  }

  // Convert a value to an int type
  dauw_int_t Value::as_int() const
  {
    if (!is_int())
      throw ValueMismatchException(fmt::format("The value {:#016x} does not represent a valid int value", value_));

    dauw_int_t int_value = (dauw_int_t)(value_ & BITMASK_VALUE);
    if (int_value & INT_NEGATIVE)
      return int_value | BITMASK_SIGNATURE;
    else
      return int_value;
  }

  // Convert a rune type to a value
  Value Value::of_rune(dauw_rune_t rune_value)
  {
    if (rune_value > RUNE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a non-existing code point", rune_value));
    if (rune_value >= RUNE_SURROGATE_MIN && rune_value <= RUNE_SURROGATE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a surrogate code point", rune_value));

    return Value((value_t)(BITMASK_QNAN | TAG_RUNE | ((value_t)rune_value & BITMASK_VALUE)));
  }

  // Return if the value represents an rune type
  bool Value::is_rune() const
  {
    return (value_ & (BITMASK_QNAN | BITMASK_TAG)) == (BITMASK_QNAN | TAG_RUNE);
  }

  // Convert a value to an rune type
  dauw_rune_t Value::as_rune() const
  {
    if (!is_rune())
      throw ValueMismatchException(fmt::format("The value {:#16x} does not represent a valid rune value", value_));

    dauw_rune_t rune_value = (dauw_rune_t)(value_ & BITMASK_VALUE);
    if (rune_value > RUNE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a non-existing code point", rune_value));
    if (rune_value >= RUNE_SURROGATE_MIN && rune_value <= RUNE_SURROGATE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a surrogate code point", rune_value));

    return rune_value;
  }

  // Convert a real type to a value
  Value Value::of_real(dauw_real_t real_value)
  {
    value_t value = *(value_t*)(&real_value);
    if ((value & BITMASK_QNAN) == BITMASK_QNAN)
      value = VAL_NAN;

    return Value(value);
  }

  // Return if the value represents a real type
  bool Value::is_real() const
  {
    return (value_ & BITMASK_QNAN) != BITMASK_QNAN;
  }

  // Return if the value represents a not-a-number real value
  bool Value::is_nan() const
  {
    return value_ == VAL_NAN;
  }

  // Convert a value to a real type
  dauw_real_t Value::as_real() const
  {
    if (!is_real())
      throw ValueMismatchException(fmt::format("The value {:#16x} does not represent a valid real value", value_));

    return *(dauw_real_t*)(&value_);
  }

  // Convert an object type to a value
  Value Value::of_obj(Obj* object_value)
  {
    return Value((value_t)(BITMASK_QNAN | BITMASK_SIGN | (value_t)(uintptr_t)object_value & BITMASK_VALUE));
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
      throw ValueMismatchException(fmt::format("The value {:#16x} does not represent a valid object value", value_));

    return (Obj*)(uintptr_t)(value_ & ~(BITMASK_QNAN | BITMASK_SIGN));
  }

  // Return the type of the value
  Type& Value::type()
  {
    if (is_nothing())
      return Type::type_nothing;
    else if (is_bool())
      return Type::type_bool;
    else if (is_int())
      return Type::type_int;
    else if (is_real())
      return Type::type_real;
    else if (is_rune())
      return Type::type_rune;
    else
      return as_obj()->type();
  }

  // Return if the value equals another value
  bool Value::operator==(const Value& other)
  {
    if (is_real() && other.is_real())
      return as_real() == other.as_real();
    else
      return value_ == other.value_;
  }
  bool Value::operator!=(const Value& other)
  {
    return !(*this == other);
  }

  // Return a string representation of the value
  string_t Value::str()
  {
    if (is_nothing())
      return "nothing";
    else if (is_bool())
      return fmt::format("{}", as_bool() ? "true" : "false");
    else if (is_int())
      return fmt::format("{}", as_int());
    else if (is_rune())
      return fmt::format("'{}'", dauw::utils::rune_pack_to_str(as_rune()));
    else if (is_real())
    {
      switch (value_)
      {
        case VAL_INF_POSITIVE: return "infinity";
        case VAL_INF_NEGATIVE: return "-infinity";
        case VAL_NAN: return "nan";
        default: return fmt::format("{:#}", as_real());
      }
    }
    else if (is_obj())
      return fmt::format("{}", *as_obj());
    else
      return fmt::format("<invalid value>");
  }
}
