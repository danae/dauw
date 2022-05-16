#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>
#include <dauw/source/location.hpp>


namespace dauw
{
  // Type efinition for a byte
  using byte_t = uint8_t;


  // Enum that defines the bytecode operation codes
  enum class Operation : byte_t
  {
    // Nop
    NOP,
    
    // Push to the stack
    PUSH_NOTHING,
    PUSH_FALSE,
    PUSH_TRUE,
    PUSH_CONSTANT,

    // Pop from the stack
    POP,

    // Unary operators
    OP_NEGATE,
    OP_LENGTH,
    OP_STRING,

    // Binary operators
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_FLOOR_DIVIDE,
    OP_MODULO,
    OP_ADD,
    OP_SUBTRACT,
    OP_RANGE,
    OP_THREEWAY,
    OP_LESS,
    OP_LESS_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_DIVISIBLE,
    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_MATCH,

    // Logical operations
    LOGICAL_NOT,
    LOGICAL_AND,
    LOGICAL_OR,

    // Return
    RETURN,
  };


  // Class that defines a chunk of bytecode
  class Chunk
  {
    private:
      // The bytecode contained in the chunk
      std::vector<byte_t> code_;

      // The constants contained in the chunk
      std::vector<Value> constants_;

      // The source locations of the bytes in the chunk
      std::vector<Location> locations_;


    public:
      // Return the length of the chunk
      size_t length();

      // Return a byte at the specified index
      byte_t at(size_t index);

      // Return a constant at the specified index
      Value constant_at(size_t index);

      // Write a byte to the chunk
      void write(byte_t byte, Location& location);
      void write(std::initializer_list<byte_t> bytes, Location& location);
      void write(Operation operation, Location& location);
      void write(Operation operation, byte_t byte, Location& location);
      void write(Operation operation, std::initializer_list<byte_t> bytes, Location& location);

      // Add a constant to the chunk
      size_t make_constant(Value value);

      // Disassemble the chunk
      size_t disassemble(size_t offset);
      size_t disassemble_simple(string_t name, size_t offset);
      size_t disassemble_constant(string_t name, size_t offset);
  };
}
