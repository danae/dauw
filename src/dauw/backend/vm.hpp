#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/string_object.hpp>
#include <dauw/internals/object.hpp>

#include <forward_list>

namespace dauw
{
  // Class that defines the virtual machine
  class VM
  {
    private:
      // Linked list of defined objects
      std::forward_list<Obj*> objects_;


    public:
      // Constructor
      VM();

      // Destructor
      ~VM();

      // Collect the marked objects
      void collect();

      // Append an object
      void append_object(Obj* object);

      // Free an object
      void free_object(Obj* object);

      // Allocate a string
      ObjString* allocate_string(const char* bytes);
  };
}
