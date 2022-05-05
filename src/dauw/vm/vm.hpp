#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>

#include <forward_list>


namespace dauw
{
  // Class that defines a virtual machine for executing Dauw bytecode
  class VM
  {
    private:
      // Linked list of defined objects
      std::forward_list<Obj*> objects_;


    public:
      // Constructor
      VM();

      // Descructor
      ~VM();

      // Allocate a string
      ObjString* allocate_string(const char* bytes);
  };
}
