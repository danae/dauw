#include "common.h"


namespace dauw
{
  using resolver = std::function<string_t(string_t)>;


  // Resolve a file name
  string_t resolve_file(string_t path)
  {
    // Create a vector of resolvers
    std::vector<resolver> resolvers;

    // Canonical path
    resolvers.push_back([](string_t path)->string_t { return path; });
    resolvers.push_back([](string_t path)->string_t { return fmt::format("{}.dauw", path); });

    // User bin directory
    resolvers.push_back([](string_t path)->string_t { return fmt::format("{}/.dauw/bin/{}", std::getenv("HOME"), path); });
    resolvers.push_back([](string_t path)->string_t { return fmt::format("{}/.dauw/bin/{}.dauw", std::getenv("HOME"), path); });

    // Iterate over the resolvers
    for (auto resolve : resolvers)
    {
      auto resolved_path = resolve(path);
      if (std::filesystem::exists(resolved_path))
        return std::filesystem::canonical(resolved_path);
    }

    // The path does not exist
    throw std::runtime_error(fmt::format("The file \"{}\" does not exist", path));
  }

  // Read the contents of a file
  string_t read_file(string_t path)
  {
    std::ifstream stream(path);
  	string_t source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    return source;
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
