#include "location.h"

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

  // Return the string representation of this location in a source string
  string_t Location::format(string_t& source)
  {
    // Get the line and column in the source
    auto lines = dauw::regex_lines(source);
    if (line_ >= lines.size())
      throw std::runtime_error(fmt::format("line {} is not present in the specified source", line_ + 1));

    auto line = lines[line_];
    if (col_ >= line.length())
      throw std::runtime_error(fmt::format("line {}, col {} is not present in the specified source", line_ + 1, col_ + 1));

    // Create a string stream
  	string_stream_t sb;

    // Format the source line
  	sb << fmt::format("{: >6d} | {}\n", line_ + 1, lines[line_]);

    // Format the source position
  	sb << "       | ";
  	for (auto i = 0; i < col_; i ++)
  		sb << " ";
  	sb << "^";

    // Return the string stream
  	return sb.str();
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
