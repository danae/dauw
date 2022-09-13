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
}
