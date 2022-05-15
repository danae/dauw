#include "type.hpp"

namespace dauw
{
  // Constructor for a type
  Type::Type(string_t name, const Type* base) : name_(name), base_(base)
  {
  }

  // Return the name of the type
  string_t& Type::name()
  {
    return name_;
  }

  // Return the base type of the type
  const Type* Type::base()
  {
    return base_;
  }

  // Return if the type equals another type
  bool Type::operator==(const Type& other)
  {
    return name_ == other.name_ && base_ == other.base_;
  }
  bool Type::operator!=(const Type& other)
  {
    return ~(*this == other);
  }
}
