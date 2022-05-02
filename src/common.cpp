#include "common.h"

namespace dauw
{
  // Repeat a string for the specified amount of times
  string_t string_repeat(string_t string, size_t times)
  {
    string_t result;
    for (auto i = 0; i < times; i ++)
      result += string;
    return result;
  }

  // Search for a regular expression in a string
  match_optional_t regex_search(regex_t pattern, string_t& string, size_t begin, size_t end)
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
  match_optional_t regex_match(regex_t pattern, string_t& string, size_t begin, size_t end)
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
  match_optional_t regex_fullmatch(regex_t pattern, string_t& string, size_t begin, size_t end)
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
  std::vector<string_t> regex_split(regex_t pattern, string_t& string)
  {
    std::vector<string_t> splits;
    std::sregex_token_iterator token(string.begin(), string.end(), pattern, -1);
    std::sregex_token_iterator end;
    for (; token != end; ++token)
      splits.push_back(*token);
    return splits;
  }

  // Split a string by newlines
  std::vector<string_t> regex_lines(string_t& string)
  {
    regex_t pattern("\\r?\\n");
    return regex_split(pattern, string);
  }

  // Replace non-overlapping occurrences of a regular expression in a string
  string_t regex_sub(regex_t pattern, string_t& replacement, string_t& string)
  {
    return std::regex_replace(string, pattern, replacement);
  }
}
