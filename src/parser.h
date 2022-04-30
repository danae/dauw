#pragma once

#include "common.h"
#include "ast.h"
#include "errors.h"
#include "token.h"


namespace dauw
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser
  {
    private:
      // The deque of tokens
      std::deque<Token> tokens_;

      // The index of the currently parsed token
      int index_ = 0;


      // Parsers for expressions
      Expr* parse_expression();

      // Parsers for operations
      Expr* parse_operation();
      Expr* parse_logic_or();
      Expr* parse_logic_and();
      Expr* parse_logic_not();
      Expr* parse_equality();
      Expr* parse_comparison();
      Expr* parse_threeway();
      Expr* parse_range();
      Expr* parse_term();
      Expr* parse_factor();
      Expr* parse_unary();

      // Parsers for primaries
      Expr* parse_primary();
      Expr* parse_call_postfix(Expr* callee);
      Expr* parse_subscript_postfix(Expr* callee);
      Expr* parse_access_postfix(Expr* callee);

      // Parsers for atoms
      Expr* parse_atom();

      // Basic parseer functionality
      Token advance();
      Token current();
      Token previous();
      bool at_end();
      bool check(std::string name);
      bool match(std::string name);
      bool match(std::initializer_list<std::string> names);
      Token consume(std::string name, std::string message);


    public:
      // Constructor
      Parser(std::deque<Token> tokens);

      // Parse a deque of tokens into an expression
      Expr* parse();
  };
}
