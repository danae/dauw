#include "location.hpp"

namespace dauw
{
  // Constructor for a location
  Location::Location(size_t line, size_t col)
    : line_(line), col_(col)
  {
  }

  // Constructor for a location at the begin of a source string
  Location::Location()
    : Location(0, 0)
  {
  }

  // Return the line of the location
  size_t& Location::line()
  {
    return line_;
  }

  // Return the column of the location
  size_t& Location::col()
  {
    return col_;
  }

  // Increase the line of the location
  void Location::increase_line_(size_t n)
  {
    line_ += n;
    col_ = 0;
  }

  // Increase the column of the location
  void Location::increase_col_(size_t n)
  {
    col_ += n;
  }

  // Return if the location equals another location
  bool Location::operator==(const Location& other)
  {
    return line_ == other.line_ && col_ == other.col_;
  }

  // Return if the location must be sorted before another location
  bool Location::operator<(const Location& other)
  {
    if (line_ < other.line_)
      return true;
    else if (col_ < other.col_)
      return true;
    else
      return false;
  }
}
