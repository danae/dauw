#include "vm.hpp"

namespace dauw
{
  // Constructor
  VM::VM(ErrorReporter* reporter)
    : reporter_(reporter)
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
  String* VM::VM::allocate_string(const char* bytes)
  {
    auto string = new String(bytes);
    objects_.push_front(string);
    return string;
  }

  // Execute a block of code
  VMResult VM::run(Code* code)
  {
    return VMResult::SUCCESS;
  }

  // Push a value onto the stack of the virtual machine
  void VM::push_stack(Value value)
  {
    stack_.push_back(value);
  }

  // Pop a value from the stack of the virtual machine
  Value VM::pop_stack()
  {
    if (stack_.empty())
      throw std::domain_error("Stack underflow");

    auto value = stack_.back();
    stack_.pop_back();
    return value;
  }
}
