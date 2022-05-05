#include "token.hpp"

namespace dauw
{
  // Constructor for a token
  Token::Token(string_t name, string_t value, Location location, size_t length)
    : name_(name), value_(value), location_(location), length_(length)
  {
  }
  Token::Token(string_t name, Location location, size_t length)
    : Token(name, "", location, length)
  {
  }
  Token::Token()
    : Token("bof", "", Location(""))
  {
  }

  // Return the name of the token
  string_t& Token::name()
  {
    return name_;
  }

  // Return the value of the token
  string_t& Token::value()
  {
    return value_;
  }

  // Return the location of the token
  Location& Token::location()
  {
    return location_;
  }

  // Return the length of the token
  size_t& Token::length()
  {
    return length_;
  }

  // Return if the token equals another token
  bool Token::operator==(const Token& other)
  {
    return name_ == other.name_ && value_ == other.value_ && location_ == other.location_ && length_ == other.length_;
  }

  // Compare the token to another token
  std::strong_ordering Token::operator<=>(const Token& other)
  {
    auto location_cmp = location_ <=> other.location_;
    if (location_cmp != std::strong_ordering::equal)
      return location_cmp;

    auto length_cmp_ = other.length_ <=> length_;
    return length_cmp_;
  }
}
