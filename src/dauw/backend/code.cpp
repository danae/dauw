#include "code.hpp"

namespace dauw
{
  // Return the length of the bytes
  size_t Code::bytes_length()
  {
    return bytes_.size();
  }

  // Return a byte at the specified index
  uint8_t Code::byte_at(size_t index)
  {
    if (index >= bytes_length())
      throw std::out_of_range(fmt::format("No byte found at index {}", index));
    return bytes_[index];
  }

  // Return the length of the constants
  size_t Code::constants_length()
  {
    return constants_.size();
  }

  // Return a constant at the specified index
  Value Code::constant_at(size_t index)
  {
    if (index >= constants_length())
      throw std::out_of_range(fmt::format("No constant found at index {}", index));
    return constants_[index];
  }

  // Return the length of the locations
  size_t Code::locations_length()
  {
    return locations_.size();
  }

  // Return a location at the specified index
  Location Code::location_at(size_t index)
  {
    if (index >= locations_length())
      throw std::out_of_range(fmt::format("No location found at index {}", index));
    return locations_[index];
  }


  // Write a byte to the code
  void Code::emit(uint8_t byte, Location& location)
  {
    bytes_.push_back(byte);
    locations_.push_back(location);
  }
  void Code::emit(std::initializer_list<uint8_t> bytes, Location& location)
  {
    for (uint8_t byte : bytes)
      emit(byte, location);
  }

  // Write an instruction to the code
  void Code::emit(Instruction operation, Location& location)
  {
    emit(static_cast<uint8_t>(operation), location);
  }

  // Add a constant to the code
  size_t Code::make_constant(Value value)
  {
    constants_.push_back(value);
    return constants_.size() - 1;
  }
}
