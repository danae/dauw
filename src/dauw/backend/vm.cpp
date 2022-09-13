#include "vm.hpp"

namespace dauw
{
  // Constructor
  VM::VM()
  {

  }

  // Destructor
  VM::~VM()
  {
    // Destroy the objects
    while (!objects_.empty())
    {
      auto object = objects_.front();
      objects_.pop_front();

      free_object(object);
    }
  }

  // Collect the marked objects
  void VM::collect()
  {

  }

  // Append an object
  void VM::append_object(Obj* object)
  {
    objects_.push_front(object);
  }

  // Free an object
  void VM::free_object(Obj* object)
  {
    delete object;
  }

  // Allocate a string
  ObjString* VM::allocate_string(const char* bytes)
  {
    auto string = new ObjString(bytes);
    objects_.push_front(string);
    return string;
  }
}
