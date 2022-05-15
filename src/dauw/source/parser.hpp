#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/ast.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>
#include <dauw/source/location.hpp>
#include <dauw/source/token.hpp>
#include <dauw/vm/vm.hpp>


namespace dauw
{
  // Class that defines a parser that converts a deque of tokens into an expression
  class Parser
  {
    private:
      // Type declaration for a parser function
      using parser_function_type = std::function<expr_ptr(Parser*)>;


      // Reference to the virtual machine
      VM* vm_;

      // Reference to the error reporter
      ErrorReporter* error_reporter_;

      // The deque of tokens
      std::deque<Token> tokens_;

      // The token that is currently being parsed
      Token current_;

      // The token that will be parsed next
      Token next_;

      // The last full line comment that has been parsed
      string_t last_line_comment_;


      // Parsers for expressions
      expr_ptr parse_script();
      expr_ptr parse_line();
      expr_ptr parse_expression();
      expr_ptr parse_def();
      expr_ptr parse_assignment();
      expr_ptr parse_control();
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
      expr_name_ptr_vector parse_parameters();
      expr_ptr_vector parse_arguments();

      // Parser helper functions
      expr_ptr parse_as_binary(std::initializer_list<string_t> op_names, parser_function_type parse_operand);
      expr_ptr parse_as_single_binary(std::initializer_list<string_t> op_names, parser_function_type parse_operand);
      expr_ptr parse_as_unary(std::initializer_list<string_t> op_names, parser_function_type parse_operand);

      // Parsers for values
      Value value_int(string_t value);
      Value value_real(string_t value);
      Value value_rune(string_t value);
      Value value_string(string_t value);
      Value value_regex(string_t value);

      // Create and report a syntax error
      SyntaxError error(Token token, string_t message);

      // Synchronize the parser after an error
      void synchronize();

      // Return if the parser has reaced the end of the tokens
      bool at_end();

      // Advance to the next token and return that token
      Token advance();

      // Check if the next token has the specified name
      bool check(string_t name);

      // Check if the next token has the specified name and advance if so
      bool match(string_t name);

      // Check if the next token has one of the specified names and advance if so
      bool match(std::initializer_list<string_t> names);

      // Consume the next token or report an error if it doesn't have the specified name
      Token consume(string_t name, string_t context = "");


    public:
      // Constructor
      Parser(VM* vm, ErrorReporter* error_reporter, std::deque<Token> tokens);

      // Parse a deque of tokens into an expression
      expr_ptr parse();
  };
}
