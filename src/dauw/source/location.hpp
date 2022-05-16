#pragma once

#include <dauw/common.hpp>


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

      // Operator overloads
      bool operator==(const Location& other);
      bool operator<(const Location& other);


      // Friend classes
      friend class Lexer;
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
}
