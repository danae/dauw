#include "object.hpp"

namespace dauw
{
  // Constructor for an object
  Obj::Obj(Type type)
    : type_(type)
  {
  }

  // Return the type of the object
  Type& Obj::type()
  {
    return type_;
  }

  // Return a string representation of the object
  string_t Obj::str()
  {
    return fmt::format("<object {} at {:#018x}>", type(), (uintptr_t)(this));
  }
}
