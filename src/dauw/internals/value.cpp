#include "value.hpp"

namespace dauw
{
  // Initialize the definitions for global values
  Value Value::value_nothing = Value::of_nothing();
  Value Value::value_false = Value::of_bool(false);
  Value Value::value_true = Value::of_bool(true);


  // Constructor for a value
  Value::Value(value_t value)
    : value_(value)
  {
  }

  // Constructor for a value from copying another value
  Value::Value(const Value& other)
    : value_(other.value_)
  {
  }

  // Constructor for a value from moving another value
  Value::Value(Value&& other)
    : value_(std::move(other.value_))
  {
  }

  // Assignment from a value from copying another value
  Value Value::operator=(const Value& other)
  {
    if (this != &other)
      value_ = other.value_;
    return *this;
  }

  // Assignment from a value from moving another value
  Value Value::operator=(Value&& other)
  {
    if (this != &other)
      value_ = std::move(other.value_);
    return *this;
  }

  // Convert a nothing type to a value
  Value Value::of_nothing()
  {
    return Value(VAL_NOTHING);
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
      throw ValueMismatchException(fmt::format("The value {:#018x} does not represent a valid nothing value", value_));
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
      throw ValueMismatchException(fmt::format("The value {:#018x} does not represent a valid bool value", value_));

    return value_ == VAL_TRUE;
  }

  // Convert an int type to a value
  Value Value::of_int(dauw_int_t int_value)
  {
    if (int_value < -(1LL << 47) || int_value > (1LL << 47) - 1)
      throw ValueOverflowException(fmt::format("The int {} exceeds the available int value storage of 48 bits", int_value));

    int_value = (int_value << 16) >> 16;
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
      throw ValueMismatchException(fmt::format("The value {:#018x} does not represent a valid int value", value_));

    dauw_int_t int_value = (dauw_int_t)(value_ & BITMASK_VALUE);
    int_value = (int_value << 16) >> 16;
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
      throw ValueMismatchException(fmt::format("The value {:#18x} does not represent a valid rune value", value_));

    dauw_rune_t rune_value = (dauw_rune_t)(value_ & BITMASK_VALUE);
    if (rune_value > RUNE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a non-existing code point", rune_value));
    if (rune_value >= RUNE_SURROGATE_MIN && rune_value <= RUNE_SURROGATE_MAX)
      throw ValueOverflowException(fmt::format("The rune U+{:06X} exceeds the valid rune range because it specifies a surrogate code point", rune_value));
    return rune_value;
  }

  // Convert a float type to a value
  Value Value::of_float(dauw_float_t float_value)
  {
    value_t value = *(value_t*)(&float_value);
    if ((value & BITMASK_QNAN) == BITMASK_QNAN)
      value = VAL_NAN;

    return Value(value);
  }

  // Return if the value represents a float type
  bool Value::is_float() const
  {
    return (value_ & BITMASK_QNAN) != BITMASK_QNAN;
  }

  // Return if the value represents a not-a-number float value
  bool Value::is_nan() const
  {
    return value_ == VAL_NAN;
  }

  // Convert a value to a float type
  dauw_float_t Value::as_float() const
  {
    if (!is_float())
      throw ValueMismatchException(fmt::format("The value {:#18x} does not represent a valid float value", value_));

    return *(dauw_float_t*)(&value_);
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
      throw ValueMismatchException(fmt::format("The value {:#18x} does not represent a valid object value", value_));

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
    else if (is_float())
      return Type::type_float;
    else if (is_rune())
      return Type::type_rune;
    else
      return as_obj()->type();
  }

  // Return if the value equals another value
  bool Value::operator==(const Value& other)
  {
    if (is_float() && other.is_float())
      return as_float() == other.as_float();
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
    else if (is_float())
    {
      if (value_ == VAL_INF_POSITIVE)
        return "infinity";
      else if (value_ == VAL_INF_NEGATIVE)
        return "-infinity";
      else if (is_nan())
        return "nan";
      else
        return fmt::format("{:#}", as_float());
    }
    else if (is_obj())
      return as_obj()->str();
    else
      return fmt::format("<invalid value>");
  }
}
