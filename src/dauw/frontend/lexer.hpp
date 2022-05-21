#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>


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
    public:
      // Type definition for the token token_list_type type
      using token_list_type = std::deque<Token>;

    private:
      // Regex patterns for comments and whitespaces
      static regex_t comment_pattern_;
      static regex_t whitespace_pattern_;


      // The source string
      string_t source_;

      // The name of the source string
      string_t source_name_;

      // Vector of rules that the lexer uses for tokenizing
      std::vector<LexerRule> rules_;

      // Reference to the error reporter
      ErrorReporter* reporter_;


      // Add a rule to the lexer
      void rule_(string_t name, regex_t pattern, std::function<string_t(match_t)> replacement);
      void rule_(string_t name, regex_t pattern, size_t replacement);
      void rule_(string_t name, regex_t pattern);


    public:
      // Constructor
      Lexer(string_t source, string_t source_name, ErrorReporter* reporter);

      // Convert a string into a token iterator
      token_list_type tokenize();
  };
}
