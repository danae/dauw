#include "string.hpp"

#include <dauw/utils/regex.hpp>

namespace dauw::utils
{
  // Convert a vector of codepoints to a string
  string_t codepoints_to_string(std::vector<uint32_t> codepoints)
  {
    string_t string;
    utf8::utf32to8(codepoints.cbegin(), codepoints.cend(), std::back_inserter(string));
    return string;
  }
  string_t codepoints_to_string(std::initializer_list<uint32_t> codepoints)
  {
    std::vector<uint32_t> codepoints_vector(codepoints);
    return codepoints_to_string(codepoints_vector);
  }
  string_t codepoints_to_string(uint32_t codepoint)
  {
    std::vector<uint32_t> codepoints_vector({codepoint});
    return codepoints_to_string(codepoints_vector);
  }

  // Convert a string to a vector of codepoints
  std::vector<uint32_t> string_to_codepoints(string_t string)
  {
    if (!utf8::is_valid(string.cbegin(), string.cend()))
      throw std::out_of_range(fmt::format("Invalid UTF-8 encoded string '{}'", string));

    std::vector<uint32_t> codepoints;
    utf8::utf8to32(string.cbegin(), string.cend(), std::back_inserter(codepoints));
    return codepoints;
  }

  // Parse a string as an integer
  dauw_int_t parse_int(string_t string)
  {
    // Remove thousand separators from the string
    string = Regex("_").substitute(string, "");

    // Check if the string contains a hexadecimal integer
    auto is_hex_match = Regex("0[Xx]").match(string, 0, REGEX_MATCH_AT_BEGIN);

    // Parse the string as an integer
    size_t end_index;
    auto int_value = (dauw_int_t)std::stoll(string, &end_index, is_hex_match.success() ? 16 : 10);
    if (end_index != string.length())
      throw std::invalid_argument(fmt::format("Unexpected character '{}' in integer", string.substr(end_index, 1)));

    // Return the parsed integer
    return int_value;
  }

  // Parse a string as a float
  dauw_float_t parse_float(string_t string)
  {
    // Remove thousand separators from the string
    string = Regex("_").substitute(string, "");

    // Parse the string as a float
    size_t end_index;
    double float_value = (dauw_float_t)std::stod(string, &end_index);
    if (end_index != string.length())
      throw std::invalid_argument(fmt::format("Unexpected character '{}' in float", string.substr(end_index, 1)));

    // Return the parsed float
    return float_value;
  }

  // Parse a string a a C-string
  const char* parse_string(string_t string)
  {
    // Unescape the string
    string = unescape(string, StringEscapeType::DOUBLE_QUOTED);

    // Return the parsed C-string
    return string.c_str();
  }

  // Parse a string as a regex pattern
  Regex parse_regex(string_t string)
  {
    // TODO: Properly parse the regex pattern

    // Return the parsed regex pattern
    return Regex(string);
  }

  // Repeat a string for the specified amount of times
  string_t repeat(string_t string, size_t times)
  {
    string_t result;
    for (auto i = 0; i < times; i ++)
      result.append(string);
    return result;
  }

  // Convert unprintable characters in a string to escape sequences
  string_t escape(string_t string, StringEscapeType type, bool ascii_only)
  {
    string_t result;

    auto codepoints = string_to_codepoints(string);
    for (auto i = 0; i < codepoints.size(); i ++)
    {
      auto codepoint = codepoints[i];
      if (codepoint == 0x5C)
        result.append("\\\\");
      else if (codepoint == 0x22)
        result.append("\\\"");
      else if (codepoint == 0x08)
        result.append("\\b");
      else if (codepoint == 0x09)
        result.append("\\t");
      else if (codepoint == 0x0A)
        result.append("\\n");
      else if (codepoint == 0x0C)
        result.append("\\f");
      else if (codepoint == 0x0D)
        result.append("\\r");
      else if (codepoint <= 0x1F || (codepoint >= 0x7F && codepoint <= 0x9F) || (ascii_only && codepoint >= 0xA0))
        result.append(fmt::format("\\u{{{}}}"), codepoint);
      else
        utf8::append(codepoint, std::back_inserter(result));
    }

    return result;
  }

  // Convert escape sequences in a string to unprintable characters
  string_t unescape(string_t string, StringEscapeType type)
  {
    auto escape_pattern = Regex("\\\\(?:(u\\{([0-9A-Fa-f]{1,6})\\})|(\")|(\')|([\\btnfr])|.)");
    auto escape_matches = escape_pattern.match_all(string);
    string_t result(string);

    for (auto match : escape_matches)
    {
      string_t result;
      if (match.group(1).success())
        // TODO: Convert unicode value to string
        result = match.group(2).value();
      else if (match.group(3).success() && type == StringEscapeType::DOUBLE_QUOTED)
        result = match.group(3).value();
      else if (match.group(4).success() && type == StringEscapeType::SINGLE_QUOTED)
        result = match.group(4).value();
      else if (match.group(5).success())
        result = match.group(5).value();
      else
        throw std::invalid_argument(fmt::format("Invalid escape sequence '{}'", match.value()));

      string = match.prefix().value() + result + match.suffix().value();
    }


    return string;
  }
}
