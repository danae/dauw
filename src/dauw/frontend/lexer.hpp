#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/utils/regex.hpp>

#include <tuple>
#include <variant>


namespace dauw::frontend
{
  // Class that defines a rule for the lexer
  class LexerRule
  {
    private:
      // The token kind of the rule
      TokenKind kind_;

      // The regular expression pattern of the rule
      utils::regex_pattern_ptr pattern_;

      // The replacement group of the rule
      std::optional<size_t> group_;


    public:
      // Constructor
      LexerRule(TokenKind kind, utils::regex_pattern_ptr pattern, size_t group);
      LexerRule(TokenKind kind, utils::regex_pattern_ptr pattern);

      // Return the token kind of the rule
      TokenKind kind();

      // Return the regular expression pattern of the rule
      utils::regex_pattern_ptr pattern();

      // Return the replacement of the rule based on a match
      string_t replace(utils::regex_match_ptr match);
  };


  // Class that defines the lexer
  class Lexer : public ReporterAware
  {
    public:
      // Type definition for a list of tokens
      using token_list_type = std::deque<Token>;


    private:
      // Regex patterns for the lexer
      static utils::regex_pattern_ptr newline_pattern_;
      static utils::regex_pattern_ptr comment_pattern_;
      static utils::regex_pattern_ptr whitespace_pattern_;
      static utils::regex_pattern_ptr ascii_identifier_pattern_;
      static utils::regex_pattern_ptr stropped_identifier_pattern_;
      static utils::regex_pattern_ptr int_pattern_;
      static utils::regex_pattern_ptr real_pattern_;
      static utils::regex_pattern_ptr rune_pattern_;
      static utils::regex_pattern_ptr string_pattern_;
      static utils::regex_pattern_ptr regex_pattern_;

      // Rules for the lexer
      static std::vector<LexerRule> rules_;


      // The source string
      string_t source_;


    public:
      // Constructor
      Lexer(Reporter* reporter, string_t source);

      // Convert a string into a token iterator
      token_list_type tokenize();
  };
}
