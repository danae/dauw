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

  // Return if the token must be sorted before another token
  bool Token::operator<(const Token& other)
  {
    if (location_ < other.location_)
      return true;
    else if (length_ > other.length_)
      return true;
    else
      return false;
  }
}
