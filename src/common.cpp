#include "common.h"


namespace dauw
{
  using resolver = std::function<std::string(std::string)>;


  // Resolve a file name
  std::string resolve_file(std::string path)
  {
    // Create a vector of resolvers
    std::vector<resolver> resolvers;

    // Canonical path
    resolvers.push_back([](std::string path)->std::string { return path; });
    resolvers.push_back([](std::string path)->std::string { return fmt::format("{}.dauw", path); });

    // User bin directory
    resolvers.push_back([](std::string path)->std::string { return fmt::format("{}/.dauw/bin/{}", std::getenv("HOME"), path); });
    resolvers.push_back([](std::string path)->std::string { return fmt::format("{}/.dauw/bin/{}.dauw", std::getenv("HOME"), path); });

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
  std::string read_file(std::string path)
  {
    std::ifstream stream(path);
  	std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
    return source;
  }


  // Search for a regular expression in a string
  std::optional<std::smatch> regex_search(std::regex pattern, std::string& string, size_t begin, size_t end)
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
  std::optional<std::smatch> regex_match(std::regex pattern, std::string& string, size_t begin, size_t end)
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
  std::optional<std::smatch> regex_fullmatch(std::regex pattern, std::string& string, size_t begin, size_t end)
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
  std::vector<std::string> regex_split(std::regex pattern, std::string& string)
  {
    std::vector<std::string> splits;
    std::sregex_token_iterator token(string.begin(), string.end(), pattern, -1);
    std::sregex_token_iterator end;
    for (; token != end; ++token)
      splits.push_back(*token);
    return splits;
  }

  // Split a string by newlines
  std::vector<std::string> regex_lines(std::string& string)
  {
    std::regex pattern("\\r?\\n");
    return regex_split(pattern, string);
  }

  // Replace non-overlapping occurrences of a regular expression in a string
  std::string regex_sub(std::regex pattern, std::string& replacement, std::string& string)
  {
    return std::regex_replace(string, pattern, replacement);
  }
}
