#pragma once

#include <regex>
#include <string>


namespace dauw
{
  // Split a string by a regex pattern
  std::vector<std::string> splitRegex(std::string source, std::regex regex);
}
