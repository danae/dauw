#pragma once

#include "common.h"


namespace dauw
{
  // Class that defines a location in a source string
  class Location
  {
    private:
      // The name of the source of the location
      std::string name_;

      // The line of the location
      size_t line_;

      // The column of the location
      size_t col_;


    public:
      // Constructor
      Location(std::string name, size_t line, size_t col)
        : name_(name), line_(line), col_(col) { }
      Location(std::string name)
        : Location(name, 0, 0) { }

      // Return the name of the source of the location
      std::string name();

      // Return the line of the location
      size_t line();

      // Return the column of the location
      size_t col();

      // Increase the line of the location
      void increase_line(size_t n = 1);

      // Increase the column of the location
      void increase_col(size_t n = 1);

      // Return the string representation of this location in a source string
      std::string format(std::string source);
  };


  // Class that defines a token in a source string
  class Token
  {
    private:
      // The name of the token
      std::string name_;

      // The value of the token
      std::string value_;

      // The location of the token
      Location location_;

      // The length of the match result of the token
      size_t length_;


    public:
      // Constructor
      Token(std::string name, std::string value, Location location, size_t length = 0)
        : name_(name), value_(value), location_(location), length_(length) { }
      Token(std::string name, Location location, size_t length = 0)
        : Token(name, "", location, length) { }

      // Return the name of the token
      std::string name();

      // Return the value of the token
      std::string value();

      // Return the location of the token
      Location location();

      // Return the length of the token
      size_t length();
  };
}


namespace fmt
{
  // Class that defines a formatter for a location
  template <>
  struct formatter<dauw::Location> : formatter<std::string_view>
  {
    template <typename FormatContext>
    auto format(dauw::Location location, FormatContext& ctx)
    {
      std::string format = fmt::format("{}, line {}, col {}", location.name(), location.line() + 1, location.col() + 1);
      return formatter<string_view>::format(format, ctx);
    }
  };


  // Class that defines a formatter for a lexer token
  template <>
  struct formatter<dauw::Token> : formatter<string_view>
  {
    template <typename FormatContext>
    auto format(dauw::Token token, FormatContext& ctx)
    {
      std::string format;
      if (!token.value().empty())
        format = fmt::format("{} \"{}\" at {}", token.name(), token.value(), token.location());
      else
        format = fmt::format("{} at {}", token.name(), token.location());
      return formatter<string_view>::format(format, ctx);
    }
  };
}
