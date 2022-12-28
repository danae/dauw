#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/source.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/utils/regex.hpp>

#include <algorithm>
#include <deque>
#include <tuple>


namespace dauw
{
  // Class that defines a rule for the lexer
  class LexerRule
  {
    private:
      // The token kind of the rule
      TokenKind kind_;

      // The regular expression pattern of the rule
      utils::Regex pattern_;

      // The replacement group of the rule
      std::optional<size_t> group_;


    public:
      // Constructor
      LexerRule(TokenKind kind, utils::Regex pattern, size_t group);
      LexerRule(TokenKind kind, utils::Regex pattern);

      // Return the token kind of the rule
      TokenKind kind();

      // Return the regular expression pattern of the rule
      utils::Regex pattern();

      // Return the replacement of the rule based on a match
      string_t replace(utils::RegexMatch match);
  };


  // Class that defines the lexer
  class Lexer : public ReporterAware
  {
    public:
      // Type definition for a list of tokens
      using token_list_type = std::deque<Token>;


    private:
      // Regex patterns for the lexer
      static utils::Regex newline_pattern_;
      static utils::Regex comment_pattern_;
      static utils::Regex whitespace_pattern_;
      static utils::Regex ascii_identifier_pattern_;
      static utils::Regex stropped_identifier_pattern_;
      static utils::Regex int_pattern_;
      static utils::Regex float_pattern_;
      static utils::Regex rune_pattern_;
      static utils::Regex string_pattern_;
      static utils::Regex regex_pattern_;

      // Rules for the lexer
      static std::vector<LexerRule> rules_;


      // The source of the lexer
      source_ptr source_;


    public:
      // Constructor
      Lexer(Reporter* reporter, source_ptr source);

      // Convert a string into a token iterator
      token_list_type tokenize();
  };
}
