#include "common.hpp"

namespace dauw
{
  // Namespace that defines string utilities
  namespace string_utils
  {
    // Convert escape sequences in a string to unprintable characters
    string_t unescape(string_t string, EscapeType escape_type)
    {
      // Define the patterns for escape sequences
      auto unescape_unicode_pattern = std::regex("\\\\(u)\\{([0-9A-Fa-f]{1,6})\\}");
      auto unescape_pattern = std::regex("\\\\(.)");

      match_optional_t unescape_match;

      // Replace all unicode escape sequences
      while ((unescape_match = regex_utils::search(unescape_unicode_pattern, string)).has_value())
      {
        auto match = unescape_match.value();
        string = match.prefix().str() + unescape_unicode_replacement(match, escape_type) + match.suffix().str();
      }

      // Replace all escape sequences
      while ((unescape_match = regex_utils::search(unescape_pattern, string)).has_value())
      {
        auto match = unescape_match.value();
        string = match.prefix().str() + unescape_replacement(match, escape_type) + match.suffix().str();
      }

      // Return the unescaped string
      return string;
    }

    // Return the replacement for an unicode escape sequence in a string to unescape
    string_t unescape_unicode_replacement(match_t match, EscapeType escape_type)
    {
      auto escape = match.str(1);
      auto value = match.str(2);

      if (escape == "u")
        return string_utils::from_rune((uint32_t)std::stoi(value, nullptr, 16));
      else
        throw std::out_of_range(fmt::format("Invalid escape sequence '{}'", match.str()));
    }

    // Return the replacement for an escape sequence in a string to unescape
    string_t unescape_replacement(match_t match, EscapeType escape_type)
    {
      auto escape = match.str(1);

      if (escape == "\\")
        return "\\";
      else if (escape == "\"" && escape_type == EscapeType::STRING)
        return escape;
      else if (escape == "'" && escape_type == EscapeType::RUNE)
        return escape;
      else if (escape == "/" && escape_type == EscapeType::REGEX)
        return escape;
      else if (escape == "b")
        return "\b";
      else if (escape == "f")
        return "\f";
      else if (escape == "n")
        return "\n";
      else if (escape == "r")
        return "\r";
      else if (escape == "t")
        return "\t";
      else
        throw std::out_of_range(fmt::format("Invalid escape sequence '{}'", match.str()));
    }


    // Convert a string to a vector of runes
    std::vector<uint32_t> to_runes(string_t string)
    {
      if (!utf8::is_valid(string.cbegin(), string.cend()))
        throw std::out_of_range(fmt::format("Invalid UTF-8 encoded string '{}'", string));

      std::vector<uint32_t> runes;
      utf8::utf8to32(string.cbegin(), string.cend(), std::back_inserter(runes));
      return runes;
    }

    // Convert a vector of runes to a string
    string_t from_runes(std::vector<uint32_t> runes)
    {
      string_t string;
      utf8::utf32to8(runes.cbegin(), runes.cend(), std::back_inserter(string));
      return string;
    }

    // Convert a rune to a string
    string_t from_rune(uint32_t rune)
    {
      std::vector<uint32_t> runes({rune});
      return string_utils::from_runes(runes);
    }


    // Repeat a string for the specified amount of times
    string_t repeat(string_t string, size_t times)
    {
      string_t result;
      for (auto i = 0; i < times; i ++)
        result += string;
      return result;
    }
  }


  // Namespace that defines regex utilities
  namespace regex_utils
  {
    // Search for a regular expression in a string
    match_optional_t search(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      if (end == 0)
        end = string.length();

      match_t match;
      if (std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern))
        return match;
      else
        return std::nullopt;
    }

    // Match a regular expression at the beginning of a string
    match_optional_t match(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      if (end == 0)
        end = string.length();

      match_t match;
      if (std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern, std::regex_constants::match_continuous))
        return match;
      else
        return std::nullopt;
    }

    // Match a regular expression in a whole string
    match_optional_t fullmatch(regex_t pattern, string_t& string, size_t begin, size_t end)
    {
      if (end == 0)
        end = string.length();

      match_t match;
      if (std::regex_match(string.cbegin() + begin, string.cbegin() + end, match, pattern))
        return match;
      else
        return std::nullopt;
    }

    // Split a string by the occurrences a regular expression
    std::vector<string_t> split(regex_t pattern, string_t& string)
    {
      std::vector<string_t> splits;
      std::sregex_token_iterator token(string.begin(), string.end(), pattern, -1);
      std::sregex_token_iterator end;
      for (; token != end; ++token)
        splits.push_back(*token);
      return splits;
    }

    // Split a string by newlines
    std::vector<string_t> split_lines(string_t& string)
    {
      regex_t pattern("\\r?\\n");
      return split(pattern, string);
    }

    // Replace non-overlapping occurrences of a regular expression in a string
    string_t replace(regex_t pattern, string_t replacement, string_t& string)
    {
      return std::regex_replace(string, pattern, replacement);
    }
  }
}
