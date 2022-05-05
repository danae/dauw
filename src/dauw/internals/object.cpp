#include "object.hpp"

namespace dauw
{
  // Constructor for an object
  Obj::Obj(ObjType type)
  {
    type_ = type;
  }

  // Return the type of the object
  ObjType& Obj::type()
  {
    return type_;
  }

  // Return a representative string representation of the object
  string_t Obj::to_string()
  {
    switch (type_)
    {
      case ObjType::STRING:
        return fmt::format("String(\"{}\")", dynamic_cast<ObjString*>(this)->c_str());
      default:
        return fmt::format("<object at address {:#012x}>", (uintptr_t)this);
    }
  }


  // Constructor for a string
  ObjString::ObjString(size_t length, char* bytes) : Obj(ObjType::STRING)
  {
    if (!utf8::is_valid(bytes, bytes + length))
      throw std::out_of_range(fmt::format("\"{}\" is not a valid UTF-8 encoded string", bytes));

    length_ = length;
    bytes_ = bytes;
  }

  // Constructor for a string that copies the specified bytes
  ObjString::ObjString(const char* bytes) : Obj(ObjType::STRING)
  {
    length_ = strlen(bytes);
    bytes_ = (char*)malloc(sizeof(char) * (length_ + 1));
    memcpy(bytes_, bytes, length_);
    bytes_[length_] = '\0';
  }

  // Constructor for a string that copies another string
  ObjString::ObjString(const ObjString& other) : ObjString(other.bytes_)
  {
  }

  // Destructor for a string
  ObjString::~ObjString()
  {
    free(bytes_);
  }

  // Convert the string to a C-style string
  const char* ObjString::c_str() const
  {
    return bytes_;
  }

  // Return an iterator over the runes in the string
  ObjString::iterator_type ObjString::rune_iterator()
  {
    return utf8::iterator(bytes_, begin_(), end_());
  }

  // Return the number of runes in the string
  size_t ObjString::rune_length()
  {
    return utf8::distance(begin_(), end_());
  }

  // Return the rune at the specified position in the string
  uint32_t ObjString::rune_at(size_t pos)
  {
    auto it = rune_iterator();
    for (auto i = 0; i < pos; it++);
    return *it;
  }


  // Return if the string equals another string
  bool ObjString::operator==(const ObjString& other)
  {
    return length_ == other.length_ && memcmp(bytes_, other.bytes_, length_) == 0;
  }
  bool ObjString::operator!=(const ObjString& other)
  {
    return !(*this == other);
  }

  // Concatenate another string at the end of the string
  ObjString ObjString::operator+(const ObjString& other)
  {
    auto length = length_ + other.length_;
    auto bytes = (char*)malloc(sizeof(char) * (length + 1));
    memcpy(bytes, bytes_, length_);
    memcpy(bytes + length_, other.bytes_, other.length_);
    bytes[length] = '\0';

    return ObjString(length, bytes);
  }
}
