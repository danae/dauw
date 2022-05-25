#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/ast_expr.hpp>
#include <dauw/ast/ast_type_expr.hpp>
#include <dauw/backend/vm.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>


namespace dauw::frontend
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser : public ReporterAware
  {
    private:
      // Type declaration for a parser function
      using parser_function_type = std::function<ast::expr_ptr(Parser*)>;


      // The tokens to parse
      Lexer::token_list_type tokens_;

      // The index of the token that is currently being parsed
      size_t index_;

      // Reference to the virtual machine
      backend::VM* vm_;

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
      ast::expr_ptr parse_infix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      ast::expr_ptr parse_infix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      ast::expr_ptr parse_prefix_op(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);
      ast::expr_ptr parse_prefix_op_single(std::initializer_list<TokenKind> op_kinds, parser_function_type parse_operand);

      // Parsers for expressions
      ast::expr_ptr parse_script();
      ast::expr_ptr parse_line();
      ast::expr_ptr parse_expression();
      ast::expr_ptr parse_def();
      ast::expr_ptr parse_assignment();
      ast::expr_ptr parse_control();
      ast::expr_ptr parse_echo();
      ast::expr_ptr parse_if();
      ast::expr_ptr parse_for();
      ast::expr_ptr parse_while();
      ast::expr_ptr parse_until();
      ast::expr_ptr parse_block();
      ast::expr_ptr parse_operation();
      ast::expr_ptr parse_logic_or();
      ast::expr_ptr parse_logic_and();
      ast::expr_ptr parse_logic_not();
      ast::expr_ptr parse_equality();
      ast::expr_ptr parse_comparison();
      ast::expr_ptr parse_threeway();
      ast::expr_ptr parse_range();
      ast::expr_ptr parse_term();
      ast::expr_ptr parse_factor();
      ast::expr_ptr parse_unary();
      ast::expr_ptr parse_primary();
      ast::expr_ptr parse_atom();
      ast::expr_ptr parse_int();
      ast::expr_ptr parse_real();
      ast::expr_ptr parse_rune();
      ast::expr_ptr parse_string();
      ast::expr_ptr parse_regex();
      ast::expr_ptr parse_sequence();
      ast::expr_ptr parse_record();
      ast::expr_ptr parse_lambda();
      ast::expr_ptr parse_grouped();

      // Parser for type expressions
      ast::type_expr_ptr parse_type();
      ast::type_expr_ptr parse_type_union();
      ast::type_expr_ptr parse_type_intersection();
      ast::type_expr_ptr parse_type_maybe();
      ast::type_expr_ptr parse_type_generic();
      ast::type_expr_ptr parse_type_grouped();

      // Parsers for specialized types
      ast::ExprFunction::parameters_type parse_parameters();
      ast::ExprSequence::sequence_type parse_arguments();
      std::vector<ast::type_expr_ptr> parse_type_arguments();


    public:
      // Constructor
      Parser(Reporter* reporter, backend::VM* vm, Lexer::token_list_type tokens);

      // Parse a deque of tokens into an expression
      ast::expr_ptr parse();
  };
}
