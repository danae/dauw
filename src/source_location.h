#pragma once

#include <deque>
#include <regex>
#include <sstream>
#include <string>
#include <fmt/core.h>
#include <fmt/format.h>

#include "common.h"


namespace dauw
{
  // Forward declarations
  class LexerToken;
  class LexerError;


  // Class that defines a location in a source string
  class Location
  {
    private:
      // The line of the location
      size_t m_line;

      // The column of the location
      size_t m_col;

    public:
      // Constructor
      Location(size_t line, size_t col);
      Location();

      // Return the string representation of this location in a source string
      std::string format(std::string source);

      // Friend functions
      friend std::deque<LexerToken> tokenize(std::string source);

      // Friend formatter struct
      friend struct fmt::formatter<Location>;
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
      std::string format = fmt::format("line {}, col {}", location.m_line + 1, location.m_col + 1);
      return formatter<string_view>::format(format, ctx);
    }
  };
}
