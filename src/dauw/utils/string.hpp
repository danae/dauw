#pragma once

#include <dauw/common.hpp>
#include <dauw/utils/regex.hpp>


namespace dauw::utils
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
  regex_pattern_ptr parse_regex(string_t string);

  // Repeat a string for the specified amount of times
  string_t repeat(string_t string, size_t times);

  // Convert unprintable characters in a string to escape sequences
  string_t escape(string_t string, EscapeType type, bool ascii_only = false);

  // Convert escape sequences in a string to unprintable characters
  string_t unescape(string_t string, EscapeType type);
}
