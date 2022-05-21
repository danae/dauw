#pragma once

#include <dauw/common.hpp>
#include <dauw/backend/chunk.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/string.hpp>
#include <dauw/internals/value.hpp>

#include <forward_list>


namespace dauw
{
  // Class that defines a virtual machine for executing Dauw bytecode
  class VM
  {
    private:
      // The stack of the virtual machine
      std::vector<Value> stack_;

      // Linked list of defined objects
      std::forward_list<Obj*> objects_;

      // Reference to the error reporter
      ErrorReporter* reporter_;


    public:
      // Constructor
      VM(ErrorReporter* reporter);

      // Descructor
      ~VM();

      // Allocate a string
      String* allocate_string(const char* bytes);

      // Run a chunk
      int run(Chunk* chunk);

      // Manipulate the stack of the virtual machine
      void push_stack(Value value);
      Value peek_stack();
      Value pop_stack();
  };
}
