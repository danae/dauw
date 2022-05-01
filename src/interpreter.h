#pragma once

#include "common.h"
#include "errors.h"
#include "lexer.h"
#include "logging.h"
#include "parser.h"


namespace dauw
{
  // Class that defines the interpreter
  class Interpreter : public ExprVisitor, public std::enable_shared_from_this<Interpreter>
  {
    private:
      int depth = 0;


    public:
      // Interpret a source string
      int interpret(string_t source_name, string_t source);

      // Interpret from a read-eval-print loop
      int interpret_from_repl();

      // Interpret from a file
      int interpret_from_file(string_t file_path);

      void print_depth();
      void print(const std::shared_ptr<dauw::Expr>& expr);

      // Visitor implementation
      void visit_literal(const std::shared_ptr<dauw::ExprLiteral>& expr);
      void visit_name(const std::shared_ptr<dauw::ExprName>& expr);
      void visit_parenthesized(const std::shared_ptr<dauw::ExprParenthesized>& expr);
      void visit_call(const std::shared_ptr<dauw::ExprCall>& expr);
      void visit_subscript(const std::shared_ptr<dauw::ExprSubscript>& expr);
      void visit_get(const std::shared_ptr<dauw::ExprGet>& expr);
      void visit_unary(const std::shared_ptr<dauw::ExprUnary>& expr);
      void visit_binary(const std::shared_ptr<dauw::ExprBinary>& expr);
      void visit_block(const std::shared_ptr<dauw::ExprBlock>& expr);
  };
}
