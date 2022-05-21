#pragma once

#include <deque>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
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
  // Namespace that defines math utilities
  namespace math_utils
  {
    // Return the signum of an integer
    int64_t sign(int64_t num);

    // Returns the floor division of two integers
    int64_t floordiv(int64_t numerator, int64_t denominator);

    // Returns the floor remainder of two integers
    int64_t floormod(int64_t numerator, int64_t denominator);

    // Returns the signum of a real
    double sign(double num);

    // Returns the floor division of two reals
    double floordiv(double numerator, double denominator);

    // Returns the floor remainder of two reals
    double floormod(double numerator, double denominator);
  }


  // Namespace that defines string utilities
  namespace string_utils
  {
    // Enum that defines the type of (un)escape to perform
    enum class EscapeType { RUNE, STRING, REGEX };


    // Convert escape sequences in a string to unprintable characters
    string_t unescape(string_t string, EscapeType escape_type);

    // Return the replacement for an unicode escape sequence in a string to unescape
    string_t unescape_unicode_replacement(match_t match, EscapeType escape_type);

    // Return the replacement for an escape sequence in a string to unescape
    string_t unescape_replacement(match_t match, EscapeType escape_type);


    // Convert a string to a vector of runes
    std::vector<uint32_t> to_runes(string_t string);

    // Convert a vector of runes to a string
    string_t from_runes(std::vector<uint32_t> runes);

    // Convert a rune to a string
    string_t from_rune(uint32_t rune);


    // Repeat a string for the specified amount of times
    string_t repeat(string_t string, size_t times);
  }


  // Namespace that defines regex utilities
  namespace regex_utils
  {
    // Search for a regular expression in a string
    match_optional_t search(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

    // Match a regular expression at the beginning of a string
    match_optional_t match(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

    // Match a regular expression in a whole string
    match_optional_t fullmatch(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

    // Split a string by the occurrences a regular expression
    std::vector<string_t> split(regex_t pattern, string_t& string);

    // Split a string by newlines
    std::vector<string_t> split_lines(string_t& string);

    // Replace non-overlapping occurrences of a regular expression in a string
    string_t replace(regex_t pattern, string_t replacement, string_t& string);
  }
}
