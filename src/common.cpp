#include "common.h"


namespace dauw
{
  // Search for a regular expression in a string
  std::optional<std::smatch> regex_search(std::regex& pattern, std::string& string, size_t begin, size_t end)
  {
    if (end == 0)
      end = string.length();

    std::smatch match;
    if (std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern))
      return match;
    else
      return std::nullopt;
  }

  // Match a regular expression at the beginning of a string
  std::optional<std::smatch> regex_match(std::regex& pattern, std::string& string, size_t begin, size_t end)
  {
    if (end == 0)
      end = string.length();

    std::smatch match;
    if (std::regex_search(string.cbegin() + begin, string.cbegin() + end, match, pattern, std::regex_constants::match_continuous))
      return match;
    else
      return std::nullopt;
  }

  // Match a regular expression in a whole string
  std::optional<std::smatch> regex_fullmatch(std::regex& pattern, std::string& string, size_t begin, size_t end)
  {
    if (end == 0)
      end = string.length();

    std::smatch match;
    if (std::regex_match(string.cbegin() + begin, string.cbegin() + end, match, pattern))
      return match;
    else
      return std::nullopt;
  }

  // Split a string by the occurrences a regular expression
  std::vector<std::string> regex_split(std::regex& pattern, std::string& string)
  {
    std::vector<std::string> splits;
    std::sregex_token_iterator token(string.begin(), string.end(), pattern, -1);
    std::sregex_token_iterator end;
    for (; token != end; ++token)
      splits.push_back(*token);
    return splits;
  }

  // Replace non-overlapping occurrences of a regular expression in a string
  std::string regex_sub(std::regex& pattern, std::string& replacement, std::string& string)
  {
    return std::regex_replace(string, pattern, replacement);
  }
}
