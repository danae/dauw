#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/expr.hpp>
#include <dauw/ast/type_expr.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/internals/string_object.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>

#include <functional>


namespace dauw::frontend
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser : public ReporterAware
  {
    private:
      // Type declaration for a parser function
      using parser_function_type = std::function<expr_ptr(Parser*)>;


      // The tokens to parse
      Lexer::token_list_type tokens_;

      // The index of the token that is currently being parsed
      size_t index_;

      // The last full line comment that has been parsed
      string_t line_comment_;


      // Basic parser functionality
      bool at_end();
      Token current();
      Token next();
      Token advance();
      bool check(TokenKind kind);
      bool match(TokenKind kind);
      bool match(std::initializer_list<TokenKind> kinds);
      Token consume(TokenKind kind, string_t context);
      void synchronize();

      // Parser helper functions
      expr_ptr parse_infix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      expr_ptr parse_infix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      expr_ptr parse_prefix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      expr_ptr parse_prefix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);

      // Parsers for expressions
      expr_ptr parse_script();
      expr_ptr parse_line();
      expr_ptr parse_expression();
      expr_ptr parse_def();
      expr_ptr parse_assignment();
      expr_ptr parse_control();
      expr_ptr parse_echo();
      expr_ptr parse_if();
      expr_ptr parse_for();
      expr_ptr parse_while();
      expr_ptr parse_until();
      expr_ptr parse_block();
      expr_ptr parse_operation();
      expr_ptr parse_logic_or();
      expr_ptr parse_logic_and();
      expr_ptr parse_logic_not();
      expr_ptr parse_equality();
      expr_ptr parse_comparison();
      expr_ptr parse_threeway();
      expr_ptr parse_range();
      expr_ptr parse_term();
      expr_ptr parse_factor();
      expr_ptr parse_unary();
      expr_ptr parse_primary();
      expr_ptr parse_atom();
      expr_ptr parse_int();
      expr_ptr parse_real();
      expr_ptr parse_rune();
      expr_ptr parse_string();
      expr_ptr parse_regex();
      expr_ptr parse_sequence();
      expr_ptr parse_record();
      expr_ptr parse_lambda();
      expr_ptr parse_grouped();

      // Parser for type expressions
      type_expr_ptr parse_type();
      type_expr_ptr parse_type_union();
      type_expr_ptr parse_type_intersection();
      type_expr_ptr parse_type_maybe();
      type_expr_ptr parse_type_generic();
      type_expr_ptr parse_type_grouped();

      // Parsers for specialized types
      ExprFunction::parameters_type parse_parameters();
      ExprSequence::sequence_type parse_arguments();
      std::vector<type_expr_ptr> parse_type_arguments();


    public:
      // Constructor
      Parser(Reporter* reporter, Lexer::token_list_type tokens);

      // Parse a deque of tokens into an expression
      expr_ptr parse();
  };
}
