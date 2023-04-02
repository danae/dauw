#pragma once

#include <dauw/common.hpp>


namespace dauw::utils
{

  // Enum that defines the type of the string to (un)escape
  enum class StringEscapeType {
    DOUBLE_QUOTED,  // Escapes double quotes
    SINGLE_QUOTED,  // Escapes single quotes
  };


  // Convert a vector of codepoints to a string
  string_t codepoints_to_string(std::vector<uint32_t> codepoints);
  string_t codepoints_to_string(std::initializer_list<uint32_t> codepoints);
  string_t codepoints_to_string(uint32_t codepoint);

  // Convert a string to a vector of codepoints
  std::vector<uint32_t> string_to_codepoints(string_t string);

  // Parse a string as another value type
  dauw_int_t parse_int(string_t string);
  dauw_float_t parse_float(string_t string);
  const char* parse_string(string_t string);
  //Regex parse_regex(string_t string);

  // Repeat a string for the specified amount of times
  string_t repeat(string_t string, size_t times);

  // Convert unprintable characters in a string to escape sequences
  string_t escape(string_t string, StringEscapeType type, bool ascii_only = false);

  // Convert escape sequences in a string to unprintable characters
  string_t unescape(string_t string, StringEscapeType type);
}
