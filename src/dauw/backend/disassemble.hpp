#pragma once

#include <dauw/common.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/backend/instruction.hpp>


namespace dauw
{
  // Class that defines a disassembler for a block of code
  class Disassembler
  {
    private:
      // The block of code to disassemble
      Code* code_;


    public:
      // Constructor
      Disassembler(Code* code);

      // Disassemble the byte at the given offset and return the next offset
      size_t disassemble(size_t offset);

      // Disassemble specific instructions
      size_t disassemble_simple(string_t name, size_t offset);
      size_t disassemble_constant(string_t name, size_t offset);
  };
}
