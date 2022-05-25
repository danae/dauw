#include "code.hpp"

namespace dauw::backend
{
  // Return the length of the bytes
  size_t Code::bytes_length()
  {
    return bytes_.size();
  }

  // Return a byte at the specified index
  Code::byte_type Code::byte_at(size_t index)
  {
    if (index >= bytes_.size())
      throw std::out_of_range(fmt::format("No byte found at index {}", index));
    return bytes_[index];
  }

  // Return a byte location at the specified index
  frontend::Location& Code::byte_location_at(size_t index)
  {
    if (index >= locations_.size())
      throw std::out_of_range(fmt::format("No byte location found at index {}", index));
    return locations_[index];
  }

  // Append a byte to the code
  size_t Code::append_byte(Code::byte_type byte, frontend::Location location)
  {
    bytes_.push_back(byte);
    locations_.push_back(location);
    return bytes_.size() - 1;
  }

  // Return the length of the constants
  size_t Code::constants_length()
  {
    return constants_.size();
  }

  // Return a constant at the specified index
  internals::Value& Code::constant_at(size_t index)
  {
    if (index >= constants_.size())
      throw std::out_of_range(fmt::format("No constant found at index {}", index));
    return constants_[index];
  }

  // Append a constant to the code
  size_t Code::append_constant(internals::Value value)
  {
    constants_.push_back(value);
    return constants_.size() - 1;
  }
}
