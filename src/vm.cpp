#include "vm.h"

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

      fmt::print(fmt::fg(fmt::color::gray), "[VM] Freeing object {}\n", object->to_string());

      delete object;
    }
  }

  // Allocate a string
  ObjString* VM::VM::allocate_string(const char* bytes)
  {
    auto string = new ObjString(bytes);

    fmt::print(fmt::fg(fmt::color::gray), "[VM] Allocating object {}\n", string->to_string());

    objects_.push_front(string);
    return string;
  }
}
