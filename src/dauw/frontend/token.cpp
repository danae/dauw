#include "token.hpp"

namespace dauw::frontend
{
  // Constructor for a token
  Token::Token(TokenKind kind, string_t value, Location location)
    : kind_(kind), value_(value), location_(location)
  {
  }
  Token::Token(TokenKind kind, Location location)
    : Token(kind, "", location)
  {
  }

  // Return the kind of the token
  TokenKind& Token::kind()
  {
    return kind_;
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

  // Return if the token equals another token
  bool Token::operator==(const Token& other)
  {
    return kind_ == other.kind_ && value_ == other.value_ && location_ == other.location_;
  }

  // Return if the token must be sorted before another token
  bool Token::operator<(const Token& other)
  {
     return location_ < other.location_;
  }
}
