#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/ast.hpp>
#include <dauw/backend/vm.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/token.hpp>


namespace dauw
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser
  {
    private:
      // Type declaration for a parser function
      using parser_function_type = std::function<expr_ptr(Parser*)>;


      // The tokens to parse
      Lexer::token_list_type tokens_;

      // The index of the token that is currently being parsed
      size_t index_;

      // References to application components
      VM* vm_;
      ErrorReporter* reporter_;

      // The last full line comment that has been parsed
      string_t line_comment_;


      // Basic parser functionality
      bool at_end();
      Token current();
      Token next();
      Token advance();
      bool check(string_t name);
      bool match(string_t name);
      bool match(std::initializer_list<string_t> names);
      Token consume(string_t name, string_t context);
      void synchronize();

      // Parser helper functions
      expr_ptr parse_infix_op(std::initializer_list<string_t> op_names, parser_function_type parse_operand);
      expr_ptr parse_infix_op_single(std::initializer_list<string_t> op_names, parser_function_type parse_operand);
      expr_ptr parse_prefix_op(std::initializer_list<string_t> op_names, parser_function_type parse_operand);
      expr_ptr parse_prefix_op_single(std::initializer_list<string_t> op_names, parser_function_type parse_operand);

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
      expr_ptr parse_simple();
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
      TypeExprGeneric::generic_type parse_type_arguments();

      // Parsers for values
      Value value_int(string_t value);
      Value value_real(string_t value);
      Value value_rune(string_t value);
      Value value_string(string_t value);
      Value value_regex(string_t value);


    public:
      // Constructor
      Parser(Lexer::token_list_type tokens, ErrorReporter* reporter, VM* vm);

      // Parse a deque of tokens into an expression
      expr_ptr parse();
  };
}
