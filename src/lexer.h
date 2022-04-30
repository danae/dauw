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
#include "token.h"


namespace dauw
{
  // Class that defines a rule to use with the lexer
  class LexerRule
  {
    private:
      // The name of the rule
      std::string name_;

      // The regex pattern of the rule
      std::regex pattern_;

      // The replacement function of this rule
      std::function<std::string(std::smatch)> replacement_;


    public:
      // Constructor
      LexerRule(std::string name, std::regex pattern, std::function<std::string(std::smatch)> replacement)
        : name_(name), pattern_(pattern), replacement_(replacement) { }
      LexerRule(std::string name, std::regex pattern, size_t replacement)
        : LexerRule(name, pattern, [replacement](std::smatch match)->std::string { return match[replacement].str(); }) { }
      LexerRule(std::string name, std::regex pattern)
        : LexerRule(name, pattern, [](std::smatch match)->std::string { return ""; }) { }

      // Return the name of the rule
      std::string name();

      // Return the pattern of the rule
      std::regex pattern();

      // Replace a match using this rule
      std::string replace(std::smatch match);
  };


  // Class that defines the lexer
  class Lexer
  {
    private:
      // Regex patterns for comments, newlines and whitespaces
      std::regex comment_pattern_;
      std::regex whitespace_pattern_;

      // LexerRules for the lexer
      std::vector<LexerRule> rules_;


      // Replacement function for an identifier
      static std::string replace_identifier_(std::smatch match);


    public:
      // Constructor
      Lexer();

      // Convert a string into a deque of tokens
      std::deque<Token> tokenize(std::string source);
  };
}
