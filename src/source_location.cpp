#include "source_location.h"

namespace dauw
{
  // Return the name of the source of the location
  string_t& Location::name()
  {
    return name_;
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

  // Compare the location to another location
  std::strong_ordering Location::operator<=>(const Location& other)
  {
    auto line_cmp = line_ <=> other.line_;
    if (line_cmp != std::strong_ordering::equal)
      return line_cmp;

    auto col_cmp = col_ <=> other.col_;
    return col_cmp;
  }
}
