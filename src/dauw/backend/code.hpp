#pragma once

#include <dauw/common.hpp>
#include <dauw/backend/instruction.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>


namespace dauw
{
  // Class that defines a code of bytecode
  class Code
  {
    private:
      // The bytes contained in the code
      std::vector<uint8_t> bytes_;

      // The constants contained in the code
      std::vector<Value> constants_;

      // The source locations of the bytes in the code
      std::vector<Location> locations_;


    public:
      // Return the length of the bytes
      size_t bytes_length();

      // Return a byte at the specified index
      uint8_t byte_at(size_t index);

      // Return the length of the constants
      size_t constants_length();

      // Return a constant at the specified index
      Value constant_at(size_t index);

      // Return the length of the locations
      size_t locations_length();

      // Return a location at the specified index
      Location location_at(size_t index);

      // Write a byte to the code
      void emit(uint8_t byte, Location& location);
      void emit(std::initializer_list<uint8_t> bytes, Location& location);

      // Write an instruction to the code
      void emit(Instruction operation, Location& location);

      // Add a constant to the code
      size_t make_constant(Value value);
  };
}
