#include "chunk.hpp"

namespace dauw
{
  // Return the length of the chunk
  size_t Chunk::length()
  {
    return code_.size();
  }

  // Return a byte at the specified index
  byte_t Chunk::at(size_t index)
  {
    return code_[index];
  }

  // Return a constant at the specified index
  Value Chunk::constant_at(size_t index)
  {
    return constants_[index];
  }

  // Write a byte to the chunk
  void Chunk::write(byte_t byte, Location& location)
  {
    code_.push_back(byte);
    locations_.push_back(location);
  }
  void Chunk::write(std::initializer_list<byte_t> bytes, Location& location)
  {
    for (byte_t byte : bytes)
      write(byte, location);
  }
  void Chunk::write(Operation operation, Location& location)
  {
    write(static_cast<byte_t>(operation), location);
  }
  void Chunk::write(Operation operation, byte_t byte, Location& location)
  {
    write(operation, location);
    write(byte, location);
  }
  void Chunk::write(Operation operation, std::initializer_list<byte_t> bytes, Location& location)
  {
    write(static_cast<byte_t>(operation), location);
    write(bytes, location);
  }

  // Add a constant to the chunk
  size_t Chunk::make_constant(Value value)
  {
    constants_.push_back(value);
    return constants_.size() - 1;
  }

  size_t Chunk::disassemble(size_t offset)
  {
    fmt::print("{:04X} {:64s} ", offset, locations_[offset]);

    auto instruction = static_cast<Operation>(code_[offset]);
    switch (instruction)
    {
      case Operation::NOP:
        return disassemble_simple("NOP", offset);

      case Operation::PUSH_NOTHING:
        return disassemble_simple("PUSH_NOTHING", offset);
      case Operation::PUSH_FALSE:
        return disassemble_simple("PUSH_FALSE", offset);
      case Operation::PUSH_TRUE:
        return disassemble_simple("PUSH_TRUE", offset);
      case Operation::PUSH_CONSTANT:
        return disassemble_constant("PUSH_CONSTANT", offset);

      case Operation::POP:
        return disassemble_constant("POP", offset);

      case Operation::OP_NEGATE:
        return disassemble_simple("OP_NEGATE", offset);
      case Operation::OP_LENGTH:
        return disassemble_simple("OP_LENGTH", offset);
      case Operation::OP_STRING:
        return disassemble_simple("OP_STRING", offset);

      case Operation::OP_MULTIPLY:
        return disassemble_simple("OP_MULTIPLY", offset);
      case Operation::OP_DIVIDE:
        return disassemble_simple("OP_DIVIDE", offset);
      case Operation::OP_FLOOR_DIVIDE:
        return disassemble_simple("OP_FLOOR_DIVIDE", offset);
      case Operation::OP_MODULO:
        return disassemble_simple("OP_MODULO", offset);
      case Operation::OP_ADD:
        return disassemble_simple("OP_ADD", offset);
      case Operation::OP_SUBTRACT:
        return disassemble_simple("OP_SUBTRACT", offset);
      case Operation::OP_RANGE:
        return disassemble_simple("OP_RANGE", offset);
      case Operation::OP_THREEWAY:
        return disassemble_simple("OP_THREEWAY", offset);
      case Operation::OP_LESS:
        return disassemble_simple("OP_LESS", offset);
      case Operation::OP_LESS_EQUAL:
        return disassemble_simple("OP_LESS_EQUAL", offset);
      case Operation::OP_GREATER:
        return disassemble_simple("OP_GREATER", offset);
      case Operation::OP_GREATER_EQUAL:
        return disassemble_simple("OP_GREATER_EQUAL", offset);
      case Operation::OP_DIVISIBLE:
        return disassemble_simple("OP_DIVISIBLE", offset);
      case Operation::OP_EQUAL:
        return disassemble_simple("OP_EQUAL", offset);
      case Operation::OP_NOT_EQUAL:
        return disassemble_simple("OP_NOT_EQUAL", offset);
      case Operation::OP_MATCH:
        return disassemble_simple("OP_MATCH", offset);

      case Operation::LOGICAL_NOT:
        return disassemble_simple("LOGICAL_NOT", offset);
      case Operation::LOGICAL_AND:
        return disassemble_simple("LOGICAL_AND", offset);
      case Operation::LOGICAL_OR:
        return disassemble_simple("LOGICAL_OR", offset);

      case Operation::RETURN:
        return disassemble_simple("RETURN", offset);

      default:
        fmt::print("Unknown opcode {:02X}\n", static_cast<byte_t>(instruction));
        return offset + 1;
    }
  }

  size_t Chunk::disassemble_simple(string_t name, size_t offset)
  {
    fmt::print("{}\n", name);
    return offset + 1;
  }

  size_t Chunk::disassemble_constant(string_t name, size_t offset)
  {
    byte_t constant = code_[offset + 1];
    fmt::print("{:24s}{}\n", name, constants_[constant]);
    return offset + 2;
  }
}
