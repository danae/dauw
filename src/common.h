#pragma once

#include <deque>
#include <iterator>
#include <regex>
#include <string>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <utf8.h>


// Defines
#ifndef DAUW_VERSION
  #define DAUW_VERSION "0.1.0"
#endif
#ifndef DAUW_GIT_BRANCH
  #define DAUW_GIT_BRANCH "unknown"
#endif
#ifndef DAUW_GIT_COMMIT_HASH
  #define DAUW_GIT_COMMIT_HASH "unknown"
#endif

// Defines for exit codes
#define DAUW_EXIT_OK 0
#define DAUW_EXIT_USAGE 64
#define DAUW_EXIT_DATAERR 65
#define DAUW_EXIT_SOFTWAREERR 70
#define DAUW_EXIT_OSERR 71
#define DAUW_EXIT_IOERR 74
#define DAUW_EXIT_CONFIG 78


// Type declarations for common std types
using string_t = std::string;
using string_view_t = std::string_view;
using regex_t = std::regex;
using match_t = std::smatch;
using match_optional_t = std::optional<match_t>;


namespace dauw
{
  // Convert a string to a vector of runes
  std::vector<char32_t> string_to_runes(string_t string);

  // Convert a vector of runes to a string
  string_t string_from_runes(std::vector<char32_t> runes);

  // Repeat a string for the specified amount of times
  string_t string_repeat(string_t string, size_t times);

  // Convert escape sequences in a string to their literal equivalent
  string_t string_unescape(string_t string);

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
  string_t regex_replace(regex_t pattern, string_t replacement, string_t& string);
}
