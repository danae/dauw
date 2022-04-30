#pragma once

#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>


namespace dauw
{
  // Resolve a file name
  std::string resolve_file(std::string path);

  // Read the contents of a file
  std::string read_file(std::string path);


  // Search for a regular expression in a string
  std::optional<std::smatch> regex_search(std::regex pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression at the beginning of a string
  std::optional<std::smatch> regex_match(std::regex pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression in a whole string
  std::optional<std::smatch> regex_fullmatch(std::regex pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Split a string by the occurrences a regular expression
  std::vector<std::string> regex_split(std::regex pattern, std::string& string);

  // Split a string by newlines
  std::vector<std::string> regex_lines(std::string& string);

  // Replace non-overlapping occurrences of a regular expression in a string
  std::string regex_sub(std::regex pattern, std::string& replacement, std::string& string);
}
