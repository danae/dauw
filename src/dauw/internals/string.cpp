#include "string.hpp"

namespace dauw
{
  // Constructor for a string
  String::String(size_t length, char* bytes)
    : Obj(ObjType::STRING)
  {
    if (!utf8::is_valid(bytes, bytes + length))
      throw std::out_of_range(fmt::format("\"{}\" is not a valid UTF-8 encoded string", bytes));

    length_ = length;
    bytes_ = bytes;
  }

  // Constructor for a string that copies the specified bytes
  String::String(const char* bytes)
    : Obj(ObjType::STRING)
  {
    length_ = strlen(bytes);
    bytes_ = (char*)malloc(sizeof(char) * (length_ + 1));
    memcpy(bytes_, bytes, length_);
    bytes_[length_] = '\0';
  }

  // Constructor for a string that copies another string
  String::String(const String& other)
     : String(other.bytes_)
  {
  }

  // Destructor for a string
  String::~String()
  {
    free(bytes_);
  }

  // Convert the string to a C-style string
  const char* String::c_str() const
  {
    return bytes_;
  }

  // Return an iterator over the runes in the string
  String::iterator_type String::rune_iterator()
  {
    return utf8::iterator(bytes_, begin_(), end_());
  }

  // Return the number of runes in the string
  size_t String::rune_length()
  {
    return utf8::distance(begin_(), end_());
  }

  // Return the rune at the specified position in the string
  uint32_t String::rune_at(size_t pos)
  {
    auto it = rune_iterator();
    for (auto i = 0; i < pos; it++);
    return *it;
  }


  // Return if the string equals another string
  bool String::operator==(const String& other)
  {
    return length_ == other.length_ && memcmp(bytes_, other.bytes_, length_) == 0;
  }
  bool String::operator!=(const String& other)
  {
    return !(*this == other);
  }

  // Concatenate another string at the end of the string
  String String::operator+(const String& other)
  {
    auto length = length_ + other.length_;
    auto bytes = (char*)malloc(sizeof(char) * (length + 1));
    memcpy(bytes, bytes_, length_);
    memcpy(bytes + length_, other.bytes_, other.length_);
    bytes[length] = '\0';

    return String(length, bytes);
  }
}
