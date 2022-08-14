#include "object.hpp"

namespace dauw::internals
{
  // Constructor for an object
  Obj::Obj(ObjKind kind, Type type)
    : kind_(kind), type_(type)
  {
  }

  // Return the kind of the object
  ObjKind& Obj::kind()
  {
    return kind_;
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
