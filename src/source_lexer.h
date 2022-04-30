#pragma once

#include <deque>
#include <functional>
#include <regex>
#include <string>
#include <vector>
#include <fmt/core.h>
#include <fmt/format.h>

#include "common.h"
#include "errors.h"
#include "source_location.h"

#define MAX_LEXER_RULES 256


namespace dauw
{
  // Forward declarations
  class LexerRule;
  class LexerToken;


  // Class that defines a rule to use with the lexer
  class LexerRule
  {
    private:
      // The name of the rule
      std::string m_name;

      // The regex pattern of the rule
      std::regex m_pattern;

      // The replacement function of this rule
      std::function<std::string(std::smatch)> m_replacement;

    public:
      // Constructor
      LexerRule(std::string name, std::regex pattern, std::function<std::string(std::smatch)> replacement);
      LexerRule(std::string name, std::regex pattern, size_t replacement);
      LexerRule(std::string name, std::regex pattern);

      // Friend functions
      friend std::deque<LexerToken> tokenize(std::string source);
  };


  // Class that defines a token to use with the lexer
  class LexerToken
  {
    private:
      // The name of the token
      std::string m_name;

      // The value of the token
      std::string m_value;

      // The location of the token
      Location m_location;

      // The length of the match result of the token
      size_t m_length;

    public:
      // Constructor
      LexerToken(std::string name, std::string value, Location location, size_t length = 0);
      LexerToken(std::string name, Location location, size_t length = 0);

      // Friend functions
      friend std::deque<LexerToken> tokenize(std::string source);

      // Friend formatter struct
      friend struct fmt::formatter<LexerToken>;
  };


  // Convert a string into a deque of tokens
  std::deque<LexerToken> tokenize(std::string source);
}


namespace fmt
{
  // Class that defines a formatter for a lexer token
  template <>
  struct formatter<dauw::LexerToken> : formatter<string_view>
  {
    template <typename FormatContext>
    auto format(dauw::LexerToken token, FormatContext& ctx)
    {
      std::string format;
      if (!token.m_value.empty())
        format = fmt::format("{} \"{}\" at {}", token.m_name, token.m_value, token.m_location);
      else
        format = fmt::format("{} at {}", token.m_name, token.m_location);
      return formatter<string_view>::format(format, ctx);
    }
  };
}
