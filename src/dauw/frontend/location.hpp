#pragma once

#include <dauw/common.hpp>


namespace dauw::frontend
{
  // Forward declarations
  class Lexer;

  // Class that defines a location in a source string
  class Location
  {
    private:
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
      Location(size_t line, size_t col);
      Location();

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
  using namespace dauw;
  using namespace dauw::frontend;

  // Class that defines a formatter for a location
  template <>
  struct formatter<Location> : formatter<string_view_t>
  {
    inline string_t stringify(Location location)
    {
      return fmt::format("line {}, col {}", location.line() + 1, location.col() + 1);
    }

    template <typename FormatContext>
    auto format(Location location, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(location), ctx);
    }
  };
}
