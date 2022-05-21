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

  // Run the chunk
  int VM::run(Chunk* chunk)
  {
    for (size_t ip = 0; ip < chunk->length(); ip ++)
    {
      auto instruction = static_cast<Operation>(chunk->at(ip));

      fmt::print(fmt::fg(fmt::color::gray), "{:04X} Stack: ", ip);
      for (auto value : stack_)
        fmt::print(fmt::fg(fmt::color::gray), "[{}] ", value);
      fmt::print(fmt::fg(fmt::color::gray), "\n");

      chunk->disassemble(ip);

      switch (instruction)
      {
        case Operation::PUSH_NOTHING: {
          push_stack(value_nothing);
          break;
        }

        case Operation::PUSH_FALSE: {
          push_stack(value_false);
          break;
        }

        case Operation::PUSH_TRUE: {
          push_stack(value_true);
          break;
        }

        case Operation::PUSH_CONSTANT: {
          auto constant_byte = chunk->at(++ ip);
          auto constant = chunk->constant_at(constant_byte);
          push_stack(constant);
          break;
        }

        case Operation::POP: {
          auto value = pop_stack();
          fmt::print(fmt::fg(fmt::color::gray), "POP {}\n", value);
          return DAUW_EXIT_OK;
        }

        case Operation::RETURN: {
          auto value = pop_stack();
          fmt::print(fmt::fg(fmt::color::gray), "RETURN {}\n", value);
          return DAUW_EXIT_OK;
        }
      }
    }

    return DAUW_EXIT_OK;
  }

  // Push a value onto the stack of the virtual machine
  void VM::push_stack(Value value)
  {
    stack_.push_back(value);
  }

  // Peek at the top value of the stack of the virtual machine
  Value VM::peek_stack()
  {
    return stack_.back();
  }

  // Pop a value from the stack of the virtual machine
  Value VM::pop_stack()
  {
    auto value = peek_stack();
    stack_.pop_back();
    return value;
  }
}
