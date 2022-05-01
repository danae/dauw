#pragma once

#include <dauw/common.h>
#include <dauw/errors.h>
#include <dauw/parser/location.h>
#include <dauw/parser/token.h>


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
      LexerRule(string_t name, regex_t pattern, std::function<string_t(match_t)> replacement)
        : name_(name), pattern_(pattern), replacement_(replacement) { }
      LexerRule(string_t name, regex_t pattern, size_t replacement)
        : LexerRule(name, pattern, [replacement](match_t match)->string_t { return match[replacement].str(); }) { }
      LexerRule(string_t name, regex_t pattern)
        : LexerRule(name, pattern, [](match_t match)->string_t { return ""; }) { }

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
      // The name of the source
      string_t name_;

      // Regex patterns for comments, newlines and whitespaces
      regex_t comment_pattern_;
      regex_t whitespace_pattern_;

      // LexerRules for the lexer
      std::vector<LexerRule> rules_;


      // Replacement function for an identifier
      static string_t replace_identifier_(match_t match);


    public:
      // Constructor
      Lexer(string_t name);

      // Convert a string into a deque of tokens
      std::deque<Token> tokenize(string_t source);
  };
}
