#include "string_object.hpp"

namespace dauw::internals
{
  // Default constructor for a string
  String::String()
    : Obj(ObjKind::STRING, Type::type_string), length_(0)
  {
    bytes_ = new char[length_ + 1];
    bytes_[0] = '\0';
  }

  // Constructor for a string from a C-string
  String::String(const char* bytes)
    : Obj(ObjKind::STRING, Type::type_string), length_(std::strlen(bytes))
  {
    auto bytes_length = std::strlen(bytes);
    if (!utf8::is_valid(bytes, bytes + bytes_length))
      throw StringException("The provided array of bytes is not a valid UTF-8 encoded string");

    length_ = bytes_length;
    bytes_ = new char[length_ + 1];
    std::memcpy(bytes_, bytes, length_);
    bytes_[length_] = '\0';
  }

  // Constructor for a string from another string
  String::String(const String& other)
    : Obj(ObjKind::STRING, Type::type_string), length_(other.length_)
  {
    bytes_ = new char[length_ + 1];
    std::memcpy(bytes_, other.bytes_, length_);
    bytes_[length_] = '\0';
  }

  // Assignment for a string from a C-string
  String& String::operator=(const char* bytes)
  {
    auto bytes_length = std::strlen(bytes);
    if (!utf8::is_valid(bytes, bytes + bytes_length))
      throw StringException("The provided array of bytes is not a valid UTF-8 encoded string");

    delete[] bytes_;

    length_ = bytes_length;
    bytes_ = new char[length_ + 1];
    std::memcpy(bytes_, bytes, length_);
    bytes_[length_] = '\0';

    return *this;
  }

  // Assignment for a string from another string
  String& String::operator=(const String& other)
  {
    length_ = other.length_;

    delete[] bytes_;

    bytes_ = new char[length_ + 1];
    std::memcpy(bytes_, other.bytes_, length_);
    bytes_[length_] = '\0';

    return *this;
  }

  // Destructor for a string
  String::~String()
  {
    delete[] bytes_;
  }

  // Append to the string from a C-string
  String& String::append(const char* bytes)
  {
    if (!*bytes)
      return *this;

    auto bytes_length = std::strlen(bytes);
    if (!utf8::is_valid(bytes, bytes + bytes_length))
      throw StringException("The provided array of bytes is not a valid UTF-8 encoded string");

    size_t pos = length_;
    bytes_ = (char*)std::realloc(bytes_, length_ + 1);
    std::memcpy(bytes_ + pos, bytes, length_ - pos);
    bytes_[length_] = '\0';

    return *this;
  }

  // Append to the string from another string
  String& String::append(const String& other)
  {
    if (other.length_ == 0)
      return *this;

    size_t pos = length_;
    bytes_ = (char*)std::realloc(bytes_, length_ + 1);
    std::memcpy(bytes_ + pos, other.bytes_, length_ - pos);
    bytes_[length_] = '\0';

    return *this;
  }

  // Return the actual characters of the string
  const char* String::c_str()
  {
    return (const char*)bytes_;
  }

  // Return the length in code points of the string
  size_t String::length()
  {
    return utf8::distance(bytes_, bytes_ + length_);
  }

  // Return the code point at the specified position of the string
  String::value_type String::at(size_t pos)
  {
    try
    {
      char* it = bytes_;
      utf8::advance(it, pos, bytes_ + length_);
      return (value_type)*it;
    }
    catch (utf8::not_enough_room& ex)
    {
      throw std::out_of_range(fmt::format("{}", pos));
    }
  }

  // Return an iterator over the code points of the string
  String::iterator_type String::begin()
  {
    return utf8::iterator(bytes_, bytes_, bytes_ + length_);
  }
  String::iterator_type String::end()
  {
    return utf8::iterator(bytes_ + length_, bytes_, bytes_ + length_);
  }

  // Compare the string to another string
  int String::compare(String& other)
  {
    iterator_type this_it = begin(), this_end = end();
    iterator_type other_it = other.begin(), other_end = other.end();

    while (true)
    {
      if (this_it == this_end && other_it == other_end)
        return 0;
      if (this_it != this_end && other_it == other_end)
        return -1;
      if (this_it == this_end && other_it != other_end)
        return 1;

      if (*this_it < *other_it)
        return -1;
      if (*this_it > *other_it)
        return 1;

      this_it ++;
      other_it ++;
    }

    return 0;
  }
}
