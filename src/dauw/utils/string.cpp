#include "string.hpp"

#include <dauw/utils/regex.hpp>

namespace dauw::utils
{
  // Pack a list of runes into a string
  string_t rune_pack_to_str(std::vector<dauw_rune_t> rune_values)
  {
    string_t string;
    utf8::utf32to8(rune_values.cbegin(), rune_values.cend(), std::back_inserter(string));
    return string;
  }
  string_t rune_pack_to_str(std::initializer_list<dauw_rune_t> rune_values)
  {
    std::vector<dauw_rune_t> runes(rune_values);
    return rune_pack_to_str(runes);
  }
  string_t rune_pack_to_str(dauw_rune_t rune_value)
  {
    std::vector<dauw_rune_t> runes({rune_value});
    return rune_pack_to_str(runes);
  }

  // Unpack a string into a list of runes
  std::vector<dauw_rune_t> rune_unpack_from_str(string_t string)
  {
    if (!utf8::is_valid(string.cbegin(), string.cend()))
      throw std::out_of_range(fmt::format("Invalid UTF-8 encoded string '{}'", string));

    std::vector<dauw_rune_t> runes;
    utf8::utf8to32(string.cbegin(), string.cend(), std::back_inserter(runes));
    return runes;
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

  // Parse a string as a real
  dauw_real_t parse_real(string_t string)
  {
    // Remove thousand separators from the string
    string = Regex("_").substitute(string, "");

    // Parse the string as a real
    size_t end_index;
    double real_value = (dauw_real_t)std::stod(string, &end_index);
    if (end_index != string.length())
      throw std::invalid_argument(fmt::format("Unexpected character '{}' in real", string.substr(end_index, 1)));

    // Return the parsed real
    return real_value;
  }

  // Parse a string as a rune
  dauw_rune_t parse_rune(string_t string)
  {
    // Unescape the string
    string = unescape(string, StringEscapeType::SINGLE_QUOTED);

    // Parse the rune literal
    auto runes = rune_unpack_from_str(string);
    if (runes.size() < 1)
      throw std::invalid_argument("Missing code point in rune");
    if (runes.size() > 1)
      throw std::invalid_argument("Unexpected extra code point(s) in rune");
    auto rune_value = runes.front();

    // Return the parsed rune
    return rune_value;
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

    auto runes = rune_unpack_from_str(string);
    for (auto i = 0; i < runes.size(); i ++)
    {
      auto rune = runes[i];
      if (rune == 0x5C)
        result.append("\\\\");
      else if (type == StringEscapeType::DOUBLE_QUOTED && rune == 0x22)
        result.append("\\\"");
      else if (type == StringEscapeType::SINGLE_QUOTED && rune == 0x27)
        result.append("\\'");
      else if (rune == 0x08)
        result.append("\\b");
      else if (rune == 0x09)
        result.append("\\t");
      else if (rune == 0x0A)
        result.append("\\n");
      else if (rune == 0x0C)
        result.append("\\f");
      else if (rune == 0x0D)
        result.append("\\r");
      else if (rune <= 0x1F || (rune >= 0x7F && rune <= 0x9F) || (ascii_only && rune >= 0xA0))
        result.append(fmt::format("\\u{{{}}}"), rune);
      else
        utf8::append(rune, std::back_inserter(result));
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
        result = rune_pack_to_str(parse_rune(match.group(2).value()));
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
