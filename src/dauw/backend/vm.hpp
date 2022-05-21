#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/backend/disassemble.hpp>
#include <dauw/backend/instruction.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/string.hpp>
#include <dauw/internals/value.hpp>

#include <forward_list>


namespace dauw
{
  // Enum that defines the result of executing code in the virtual machine
  enum class VMResult {
    SUCCESS,
    COMPILE_ERROR,
    RUNTIME_ERROR,
  };


  // Class that defines the virtual machine
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

      // Execute a block of code
      VMResult run(Code* code);

      // Push a value onto the stack of the virtual machine
      void push_stack(Value value);

      // Pop a value from the stack of the virtual machine
      Value pop_stack();

  };
}
