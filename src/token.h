#pragma once

#include "common.h"


namespace dauw
{
  // Forward declarations
  class Lexer;

  // Class that defines a location in a source string
  class Location
  {
    private:
      // The name of the source of the location
      string_t name_;

      // The line of the location
      size_t line_;

      // The column of the location
      size_t col_;


      // Increase the line of the location
      void increase_line_(size_t n = 1);

      // Increase the column of the location
      void increase_col_(size_t n = 1);


    public:
      // Constructor
      Location(string_t name, size_t line, size_t col)
        : name_(name), line_(line), col_(col) { }
      Location(string_t name)
        : Location(name, 0, 0) { }

      // Return the name of the source of the location
      string_t& name();

      // Return the line of the location
      size_t& line();

      // Return the column of the location
      size_t& col();

      // Return the string representation of this location in a source string
      string_t format(string_t& source);

      // Operator overloads
      bool operator==(const Location& other);
      std::strong_ordering operator<=>(const Location& other);


      // Friend classes
      friend class Lexer;
  };


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
  // Class that defines a formatter for a location
  template <>
  struct formatter<dauw::Location> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(dauw::Location location, FormatContext& ctx)
    {
      string_t format = fmt::format("{}, line {}, col {}", location.name(), location.line() + 1, location.col() + 1);
      return formatter<string_view_t>::format(format, ctx);
    }
  };


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
