#include "common.hpp"

namespace dauw
{
  // Namespace that defines math utility functions
  namespace utils::math
  {
    // Return the sign of an integer
    dauw_int_t sign(dauw_int_t n)
    {
      return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
    }

    // Returns the sign of a real
    dauw_real_t sign(dauw_real_t n)
    {
      if (std::isnan(n))
        return n;
      else
        return (n > 0.0) ? 1.0 : ((n < 0.0) ? -1.0 : 0.0);
    }

    // Returns the floor division of two integers
    dauw_int_t floordiv(dauw_int_t numerator, dauw_int_t denominator)
    {
      if (denominator == 0)
        throw std::domain_error("Division by zero");
      else
        return (int)(std::floor((double)numerator / (double)denominator));
    }

    // Returns the floor remainder of two integers
    dauw_int_t floormod(dauw_int_t numerator, dauw_int_t denominator)
    {
      auto quotient = floordiv(numerator, denominator);
      return numerator - quotient * denominator;
    }

    // Returns the floor division of two reals
    dauw_real_t floordiv(dauw_real_t numerator, dauw_real_t denominator)
    {
      if (std::isnan(numerator))
        return numerator;
      else if (std::isnan(denominator))
        return denominator;
      else if (denominator == 0.0)
        throw std::domain_error("Division by zero");
      else
        return std::floor(numerator / denominator);
    }

    // Returns the floor remainder of two reals
    dauw_real_t floormod(dauw_real_t numerator, dauw_real_t denominator)
    {
      auto quotient = floordiv(numerator, denominator);
      return numerator - quotient * denominator;
    }
  }

  // --------------------------------------------------------------------------

  // Namespace that defines string utility functions
  namespace utils::string
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
      string = regex::replace(regex_t("_"), "", string);

      // Check if the string contains a hexadecimal integer
      auto is_hex_match = regex::match(regex_t("0[Xx]"), string);
      auto is_hex = !is_hex_match.empty();

      // Parse the string as an integer
      size_t end_index;
      auto int_value = (dauw_int_t)std::stoll(string, &end_index, is_hex ? 16 : 10);
      if (end_index != string.length())
        throw std::invalid_argument(fmt::format("Unexpected character '{}' in integer", string.substr(end_index, 1)));

      // Return the parsed integer
      return int_value;
    }

    // Parse a string as a real
    dauw_real_t parse_real(string_t string)
    {
      // Remove thousand separators from the string
      string = regex::replace(regex_t("_"), "", string);

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
      string = unescape(string, EscapeType::SINGLE_QUOTED);

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
      string = unescape(string, EscapeType::DOUBLE_QUOTED);

      // Return the parsed C-string
      return string.c_str();
    }

    // Parse a string as a regex pattern
    regex_t parse_regex(string_t string)
    {
      // TODO: Properly parse the regex pattern

      // Unescape the string
      string = unescape(string, EscapeType::DOUBLE_QUOTED);

      // Return the parsed regex pattern
      return regex_t(string);
    }

    // Repeat a string for the specified amount of times
    string_t repeat(string_t string, size_t times)
    {
      string_t result;
      for (auto i = 0; i < times; i ++)
        result.append(string);
      return result;
    }

    // Split a string by newlines
    std::vector<string_t> split_lines(string_t& string)
    {
      return regex::split(regex_t(("\\r?\\n")), string);
    }

    // Convert unprintable characters in a string to escape sequences
    string_t escape(string_t string, EscapeType type, bool ascii_only)
    {
      string_t result;

      auto runes = rune_unpack_from_str(string);
      for (auto i = 0; i < runes.size(); i ++)
      {
        auto rune = runes[i];
        if (rune == 0x5C)
          result.append("\\\\");
        else if (type == EscapeType::DOUBLE_QUOTED && rune == 0x22)
          result.append("\\\"");
        else if (type == EscapeType::SINGLE_QUOTED && rune == 0x27)
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
    string_t unescape(string_t string, EscapeType type)
    {
      regex_t escape_pattern("\\\\(?:(u\\{([0-9A-Fa-f]{1,6})\\})|(\")|(\')|([\\btnfr])|.)");
      string_t result(string);

      std::regex_iterator<string_t::iterator> escape_begin(string.begin(), string.end(), escape_pattern);
      std::regex_iterator<string_t::iterator> escape_end;
      for (auto match = escape_begin; match != escape_end; match ++)
      {
        string_t result;
        if (match->length(1) > 0)
          result = rune_pack_to_str(parse_rune(match->str(2)));
        else if (match->length(3) > 0 && type == EscapeType::DOUBLE_QUOTED)
          result = match->str(3);
        else if (match->length(4) > 0 && type == EscapeType::SINGLE_QUOTED)
          result = match->str(4);
        else if (match->length(5) > 0)
          result = match->str(5);
        else
          throw std::invalid_argument(fmt::format("Invalid escape sequence '{}'", match->str()));

        string = match->prefix().str() + result + match->suffix().str();
      }

      return string;
    }
  }

  // --------------------------------------------------------------------------

  // Namespace that defines regex utility functions
  namespace utils::regex
  {
    // Search for a regular expression in a string
    match_t search(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      end = (end == 0) ? string.length() : end;
      match_t match;
      std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern);
      return match;
    }

    // Match a regular expression at the beginning of a string
    match_t match(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      end = (end == 0) ? string.length() : end;
      match_t match;
      std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern, std::regex_constants::match_continuous);
      return match;
    }

    // Match a regular expression in a whole string
    match_t fullmatch(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      end = (end == 0) ? string.length() : end;
      match_t match;
      std::regex_match(string.cbegin() + begin, string.cbegin() + end, match, pattern);
      return match;
    }

    // Replace non-overlapping occurrences of a regular expression in a string
    string_t replace(regex_t pattern, string_t replacement, string_t& string)
    {
      return std::regex_replace(string, pattern, replacement);
    }

    // Split a string by the occurrences a regular expression
    std::vector<string_t> split(regex_t pattern, string_t& string)
    {
      std::vector<string_t> splits;
      std::sregex_token_iterator token(string.begin(), string.end(), pattern, -1);
      std::sregex_token_iterator end;
      while (token != end)
        splits.push_back(*token++);
      return splits;
    }
  }
}
