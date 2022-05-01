#pragma once

#include <compare>
#include <cstdlib>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <optional>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>


// Type declarations for common std types
using string_t = std::string;
using string_stream_t = std::stringstream;
using string_view_t = std::string_view;
using regex_t = std::regex;
using match_t = std::smatch;
using match_optional_t = std::optional<match_t>;


namespace dauw
{
  // Resolve a file name
  string_t resolve_file(string_t path);

  // Read the contents of a file
  string_t read_file(string_t path);


  // Search for a regular expression in a string
  match_optional_t regex_search(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression at the beginning of a string
  match_optional_t regex_match(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

  // Match a regular expression in a whole string
  match_optional_t regex_fullmatch(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

  // Split a string by the occurrences a regular expression
  std::vector<string_t> regex_split(regex_t pattern, string_t& string);

  // Split a string by newlines
  std::vector<string_t> regex_lines(string_t& string);

  // Replace non-overlapping occurrences of a regular expression in a string
  string_t regex_sub(regex_t pattern, string_t& replacement, string_t& string);
}
