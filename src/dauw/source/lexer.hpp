#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/source/location.hpp>
#include <dauw/source/token.hpp>


namespace dauw
{
  // Class that defines a rule to use with the lexer
  class LexerRule
  {
    private:
      // The name of the rule
      string_t name_;

      // The regex pattern of the rule
      regex_t pattern_;

      // The replacement function of this rule
      std::function<string_t(match_t)> replacement_;


    public:
      // Constructor
      LexerRule(string_t name, regex_t pattern, std::function<string_t(match_t)> replacement);

      // Return the name of the rule
      string_t name();

      // Return the pattern of the rule
      regex_t pattern();

      // Replace a match using this rule
      string_t replace(match_t match);
  };


  // Class that defines the lexer
  class Lexer
  {
    private:
      // The source string
      string_t source_;
      string_t source_name_;

      // Regex patterns for comments, newlines and whitespaces
      regex_t comment_pattern_;
      regex_t whitespace_pattern_;

      // LexerRules for the lexer
      std::vector<LexerRule> rules_;


      // Add a rule to the lexer
      void rule_(string_t name, regex_t pattern, std::function<string_t(match_t)> replacement);
      void rule_(string_t name, regex_t pattern, size_t replacement);
      void rule_(string_t name, regex_t pattern);


    public:
      // Constructor
      Lexer(string_t source, string_t source_name);

      // Convert a string into a deque of tokens
      std::deque<Token> tokenize();
  };
}
