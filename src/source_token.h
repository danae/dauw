#pragma once

#include "common.h"
#include "source_location.h"


namespace dauw
{
  // Class that defines a token in a source string
  class Token
  {
    private:
      // The name of the token
      string_t name_;

      // The value of the token
      string_t value_;

      // The location of the token
      Location location_;

      // The length of the match result of the token
      size_t length_;


    public:
      // Constructor
      Token(string_t name, string_t value, Location location, size_t length = 0)
        : name_(name), value_(value), location_(location), length_(length) { }
      Token(string_t name, Location location, size_t length = 0)
        : Token(name, "", location, length) { }
      Token()
        : Token("bof", "", Location("")) { }

      // Return the name of the token
      string_t& name();

      // Return the value of the token
      string_t& value();

      // Return the location of the token
      Location& location();

      // Return the length of the token
      size_t& length();

      // Operator overloads
      bool operator==(const Token& other);
      std::strong_ordering operator<=>(const Token& other);
  };
}


namespace fmt
{
  // Class that defines a formatter for a lexer token
  template <>
  struct formatter<dauw::Token> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(dauw::Token token, FormatContext& ctx)
    {
      string_t format;
      if (!token.value().empty())
        format = fmt::format("{} \"{}\" at {}", token.name(), token.value(), token.location());
      else
        format = fmt::format("{} at {}", token.name(), token.location());
      return formatter<string_view_t>::format(format, ctx);
    }
  };
}
