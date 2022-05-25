#pragma once

#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <optional>
#include <regex>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <utf8.h>


// Defines for versions
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


namespace dauw
{
  // Type definitions for common types
  using string_t = std::string;
  using string_view_t = std::string_view;
  using regex_t = std::regex;
  using match_t = std::smatch;

  // Type definitions for internal values
  using dauw_bool_t = bool;
  using dauw_int_t = int64_t;
  using dauw_real_t = double;
  using dauw_rune_t = uint32_t;


  // Class that defines a base for all exceptions and errors
  class Exception
  {
    private:
      // The message of the exception
      string_t message_;

      // The previous exception that caused the exception to happen
      Exception* previous_;


    public:
      // Constructor
      inline Exception(string_t message, Exception* previous) : message_(message), previous_(previous) {}
      inline Exception(string_t message) : Exception(message, nullptr) {}

      // Return the message of the exception
      inline string_t message() { return message_; }

      // Return the previous exception that caused the exception to happen
      inline Exception* previous() { return previous_; }
  };


  // Namespace that defines math utility functions
  namespace utils::math
  {
    // Return the sign of a number
    dauw_int_t sign(dauw_int_t n);
    dauw_real_t sign(dauw_real_t n);

    // Returns the floor division quotient and remainder of two numbers
    dauw_int_t floordiv(dauw_int_t numerator, dauw_int_t denominator);
    dauw_int_t floormod(dauw_int_t numerator, dauw_int_t denominator);
    dauw_real_t floordiv(dauw_real_t numerator, dauw_real_t denominator);
    dauw_real_t floormod(dauw_real_t numerator, dauw_real_t denominator);
  }


  // Namespace that defines string utility functions
  namespace utils::string
  {
    // Enum that defines the type of the string to (un)escape
    enum class EscapeType {
      DOUBLE_QUOTED,  // Escapes double quotes
      SINGLE_QUOTED,  // Escapes single quotes
    };

    // Pack a list of runes into a string
    string_t rune_pack_to_str(std::vector<dauw_rune_t> rune_values);
    string_t rune_pack_to_str(std::initializer_list<dauw_rune_t> rune_values);
    string_t rune_pack_to_str(dauw_rune_t rune_value);

    // Unpack a string into a list of runes
    std::vector<dauw_rune_t> rune_unpack_from_str(string_t string);

    // Parse a string as another value type
    dauw_int_t parse_int(string_t string);
    dauw_real_t parse_real(string_t string);
    dauw_rune_t parse_rune(string_t string);
    const char* parse_string(string_t string);
    regex_t parse_regex(string_t string);

    // Repeat a string for the specified amount of times
    string_t repeat(string_t string, size_t times);

    // Split a string by newlines (\r?\n)
    std::vector<string_t> split_lines(string_t& string);

    // Convert unprintable characters in a string to escape sequences
    string_t escape(string_t string, EscapeType type, bool ascii_only = false);

    // Convert escape sequences in a string to unprintable characters
    string_t unescape(string_t string, EscapeType type);
  }


  // Namespace that defines regex utility functions
  namespace utils::regex
  {
    // Search for or match a regular expression in a string
    match_t search(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);
    match_t match(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);
    match_t fullmatch(regex_t pattern, string_t& string, size_t begin = 0, size_t end = 0);

    // Replace non-overlapping occurrences of a regular expression in a string
    string_t replace(regex_t pattern, string_t replacement, string_t& string);

    // Split a string by the occurrences a regular expression
    std::vector<string_t> split(regex_t pattern, string_t& string);
  }
}
