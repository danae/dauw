#include "token.h"


namespace dauw
{
  // Return the line of the location
  size_t Location::line()
  {
    return line_;
  }

  // Return the column of the location
  size_t Location::col()
  {
    return col_;
  }

  // Increase the line of the location
  void Location::increase_line(size_t n)
  {
    line_ += n;
    col_ = 0;
  }

  // Increase the column of the location
  void Location::increase_col(size_t n)
  {
    col_ += n;
  }

  // Return the string representation of this location in a source string
  std::string Location::format(std::string source)
  {
    // Get the line and column in the source
    auto lines = dauw::regex_lines(source);
    if (line_ >= lines.size())
      throw std::runtime_error(fmt::format("line {} is not present in the specified source", line_));

    auto line = lines[line_];
    if (col_ >= line.length())
      throw std::runtime_error(fmt::format("line {}, col {} is not present in the specified source", line_, col_));

    // Create a string stream
  	std::stringstream sb;

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


  // Return the name of the token
  std::string Token::name()
  {
    return name_;
  }

  // Return the value of the token
  std::string Token::value()
  {
    return value_;
  }

  // Return the location of the token
  Location Token::location()
  {
    return location_;
  }

  // Return the length of the token
  size_t Token::length()
  {
    return length_;
  }
}
