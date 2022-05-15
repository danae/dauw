#include "vm.hpp"

namespace dauw
{
  // Constructor
  VM::VM()
  {
  }

  // Descructor
  VM::~VM()
  {
    while (!objects_.empty())
    {
      auto object = objects_.front();
      objects_.pop_front();

      delete object;
    }
  }

  // Allocate a string
  ObjString* VM::VM::allocate_string(const char* bytes)
  {
    auto string = new ObjString(bytes);

    objects_.push_front(string);
    return string;
  }
}
