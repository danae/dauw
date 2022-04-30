#include "source_location.h"


namespace dauw
{
  // Constructor for a location
  Location::Location(size_t line, size_t col)
  {
    m_line = line;
    m_col = col;
  }
  Location::Location()
  {
    m_line = 0;
    m_col = 0;
  }

  // Return the string representation of this location in a source string
  std::string Location::format(std::string source)
  {
    std::regex newline_pattern("\\r?\\n");
    auto lines = dauw::regex_split(newline_pattern, source);
    if (lines.size() < m_line)
      return "";

  	std::stringstream sb;
  	sb << fmt::format("{: >6d} | {}\n", m_line + 1, lines[m_line]);
  	sb << "       | ";
  	for (auto i = 0; i < m_col; i ++)
  		sb << " ";
  	sb << "^";
  	return sb.str();
  }
}
