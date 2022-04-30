#pragma once

#include <optional>
#include <regex>
#include <string>


namespace dauw
{
  // Search for a regular expression in a string
  std::optional<std::smatch> regex_search(std::regex& pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression at the beginning of a string
  std::optional<std::smatch> regex_match(std::regex& pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression in a whole string
  std::optional<std::smatch> regex_fullmatch(std::regex& pattern, std::string& string, size_t begin = 0, size_t end = 0);

  // Split a string by the occurrences a regular expression
  std::vector<std::string> regex_split(std::regex& pattern, std::string& string);

  // Replace non-overlapping occurrences of a regular expression in a string
  std::string regex_sub(std::regex& pattern, std::string& replacement, std::string& string);
}
