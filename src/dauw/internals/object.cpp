#include "object.hpp"

namespace dauw
{
  // Constructor for an object
  Obj::Obj(ObjType type)
  {
    type_ = type;
  }

  // Return the type of the object
  ObjType& Obj::type()
  {
    return type_;
  }

  // Return a representative string representation of the object
  string_t Obj::to_string()
  {
    return fmt::format("<object at address {:#012x}>", (uintptr_t)this);
    /*switch (type_)
    {
      case ObjType::STRING:
        return fmt::format("\"{}\"", dynamic_cast<String*>(this)->c_str());
      default:
        return fmt::format("<object at address {:#012x}>", (uintptr_t)this);
    }*/
  }
}
