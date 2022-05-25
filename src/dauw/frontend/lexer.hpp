#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>

#include <tuple>
#include <variant>


namespace dauw::frontend
{
 // Class that defines the lexer
  class Lexer : public ReporterAware
  {
    public:
      // Type definition for a rule
      using rule_function_type = std::function<string_t(match_t)>;
      using rule_replacement_type = std::variant<std::monostate, int, rule_function_type>;
      using rule_type = std::tuple<TokenKind, regex_t, rule_replacement_type>;

      // Type definition for a matched token
      using matched_token_type = std::tuple<Token, size_t>;

      // Type definition for a list of tokens
      using token_list_type = std::deque<Token>;


    private:
      // Regex patterns for the lexer
      static regex_t comment_pattern_;
      static regex_t whitespace_pattern_;
      static regex_t identifier_pattern_;
      static regex_t int_pattern_;
      static regex_t real_pattern_;
      static regex_t rune_pattern_;
      static regex_t string_pattern_;
      static regex_t regex_pattern_;

      // Rule functions for the lexer
      static rule_function_type identifier_function_;

      // Rules for the lexer
      static std::vector<rule_type> rules_;


      // The source string
      string_t source_;


    public:
      // Constructor
      Lexer(Reporter* reporter, string_t source);

      // Convert a string into a token iterator
      token_list_type tokenize();
  };
}
