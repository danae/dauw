#pragma once

#include "common.h"
#include "ast.h"
#include "errors.h"
#include "internals_value.h"
#include "source_location.h"
#include "source_token.h"


namespace dauw
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser
  {
    private:
      // The deque of tokens
      std::deque<Token> tokens_;

      // The token that is currently being parsed
      Token current_;

      // The token that will be parsed next
      Token next_;


      // Parsers for expressions
      std::shared_ptr<Expr> parse_expression();

      // Parsers for operations
      std::shared_ptr<Expr> parse_operation();
      std::shared_ptr<Expr> parse_logic_or();
      std::shared_ptr<Expr> parse_logic_and();
      std::shared_ptr<Expr> parse_logic_not();
      std::shared_ptr<Expr> parse_equality();
      std::shared_ptr<Expr> parse_comparison();
      std::shared_ptr<Expr> parse_threeway();
      std::shared_ptr<Expr> parse_range();
      std::shared_ptr<Expr> parse_term();
      std::shared_ptr<Expr> parse_factor();
      std::shared_ptr<Expr> parse_unary();

      // Parsers for primaries
      std::shared_ptr<Expr> parse_primary();
      std::shared_ptr<Expr> parse_call_postfix(std::shared_ptr<Expr> callee);
      std::shared_ptr<Expr> parse_subscript_postfix(std::shared_ptr<Expr> callee);
      std::shared_ptr<Expr> parse_access_postfix(std::shared_ptr<Expr> callee);

      // Parsers for atoms
      std::shared_ptr<Expr> parse_atom();
      std::shared_ptr<Expr> parse_int();
      std::shared_ptr<Expr> parse_real();
      std::shared_ptr<Expr> parse_rune();
      std::shared_ptr<Expr> parse_string();
      std::shared_ptr<Expr> parse_regex();

      // Basic parseer functionality
      bool at_end();
      Token advance();
      bool check(string_t name);
      bool match(string_t name);
      bool match(std::initializer_list<string_t> names);
      Token consume(string_t name, string_t message);


    public:
      // Constructor
      Parser(std::deque<Token> tokens);

      // Parse a deque of tokens into an expression
      std::shared_ptr<Expr> parse();
  };
}
