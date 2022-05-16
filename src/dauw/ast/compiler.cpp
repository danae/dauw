#include "compiler.hpp"

namespace dauw
{
  // Constructor for the compiler
  Compiler::Compiler(Chunk* chunk)
    : chunk_(chunk)
  {
  }

  // Compile an expression
  void Compiler::compile(const expr_ptr& expr)
  {
    expr->accept(shared_from_this());
  }

  // Visit a literal expression
  void Compiler::visit_literal(const expr_literal_ptr& expr)
  {
    auto value = expr->value();

    if (value.is_nothing())
      chunk_->write(Operation::PUSH_NOTHING, expr->location());
    else if (value.is_false())
      chunk_->write(Operation::PUSH_FALSE, expr->location());
    else if (value.is_true())
      chunk_->write(Operation::PUSH_TRUE, expr->location());
    else
      chunk_->write(Operation::PUSH_CONSTANT, chunk_->make_constant(value), expr->location());
  }

  // Visit a sequence expression
  void Compiler::visit_sequence(const expr_sequence_ptr& expr)
  {
    // TODO
    //for (auto item : *expr)
    //  evaluate(item);
  }

  // Visit a record expression
  void Compiler::visit_record(const expr_record_ptr& expr)
  {
    // TODO
    //for (auto item : *expr)
    //  evaluate(std::get<1>(item));
  }

  // Visit a name expression
  void Compiler::visit_name(const expr_name_ptr& expr)
  {
    // TODO
  }

  // Visit a function expression
  void Compiler::visit_function(const expr_function_ptr& expr)
  {
    // TODO
  }

  // Visit a function parameter expression
  void Compiler::visit_function_parameter(const expr_function_parameter_ptr& expr)
  {
    // TODO
  }

  // Visit a grouped expression
  void Compiler::visit_grouped(const expr_grouped_ptr& expr)
  {
    compile(expr->expr());
  }

  // Visit a call expression
  void Compiler::visit_call(const expr_call_ptr& expr)
  {
    // Compile and validate the callee
    compile(expr->callee());

    // Compile and validate the arguments
    compile(expr->arguments());

    // TODO: Call the callable
  }

  // Visit a get expression
  void Compiler::visit_get(const expr_get_ptr& expr)
  {
    // Compile and validate the object
    compile(expr->object());
  }

  // Visit an unary expression
  void Compiler::visit_unary(const expr_unary_ptr& expr)
  {
    // Check if the expression is a logical not expression


    // Compile the operand
    compile(expr->right());

    // Check and compile the operator
    // TODO: Add type checking
    if (expr->op() == "-")
      chunk_->write(Operation::OP_NEGATE, expr->location());
    else if (expr->op() == "#")
      chunk_->write(Operation::OP_LENGTH, expr->location());
    else if (expr->op() == "$")
      chunk_->write(Operation::OP_STRING, expr->location());
  }

  // Visit a binary expression
  void Compiler::visit_binary(const expr_binary_ptr& expr)
  {
    // Check if the expression is a logical and expression

    // Check if the expression is a logical or expression

    // Compile the operands
    compile(expr->left());
    compile(expr->right());

    // Check and compile the operator
    // TODO: Add type checking
    if (expr->op() == "*")
      chunk_->write(Operation::OP_MULTIPLY, expr->location());
    else if (expr->op() == "/")
      chunk_->write(Operation::OP_DIVIDE, expr->location());
    else if (expr->op() == "//")
      chunk_->write(Operation::OP_FLOOR_DIVIDE, expr->location());
    else if (expr->op() == "%")
      chunk_->write(Operation::OP_MODULO, expr->location());
    else if (expr->op() == "+")
      chunk_->write(Operation::OP_ADD, expr->location());
    else if (expr->op() == "-")
      chunk_->write(Operation::OP_SUBTRACT, expr->location());
    else if (expr->op() == "..")
      chunk_->write(Operation::OP_RANGE, expr->location());
    else if (expr->op() == "<=>")
      chunk_->write(Operation::OP_THREEWAY, expr->location());
    else if (expr->op() == "<")
      chunk_->write(Operation::OP_LESS, expr->location());
    else if (expr->op() == "<=")
      chunk_->write(Operation::OP_LESS_EQUAL, expr->location());
    else if (expr->op() == ">")
      chunk_->write(Operation::OP_GREATER, expr->location());
    else if (expr->op() == ">=")
      chunk_->write(Operation::OP_GREATER_EQUAL, expr->location());
    else if (expr->op() == "%%")
      chunk_->write(Operation::OP_DIVISIBLE, expr->location());
    else if (expr->op() == "==")
      chunk_->write(Operation::OP_EQUAL, expr->location());
    else if (expr->op() == "<>")
      chunk_->write(Operation::OP_NOT_EQUAL, expr->location());
    else if (expr->op() == "~")
      chunk_->write(Operation::OP_MATCH, expr->location());
  }

  // Visit an if expression
  void Compiler::visit_if(const expr_if_ptr& expr)
  {
    // TODO
  }

  // Visit a for expression
  void Compiler::visit_for(const expr_for_ptr& expr)
  {
    // TODO
  }

  // Visit a while expression
  void Compiler::visit_while(const expr_while_ptr& expr)
  {
    // TODO
  }

  // Visit an until expression
  void Compiler::visit_until(const expr_until_ptr& expr)
  {
    // TODO
  }

  // Visit a block expression
  void Compiler::visit_block(const expr_block_ptr& expr)
  {
    // Iterate over the sub-expressions and compile them in order
    for (auto subexpr : *expr)
      compile(subexpr);
  }

  // Visit a def expression
  void Compiler::visit_def(const expr_def_ptr& expr)
  {
    // Compile the value
    compile(expr->value());

    // TODO
  }

  // Visit a name type expression
  void Compiler::visit_type_name(const type_expr_name_ptr& expr)
  {
    // TODO
  }

  // Visit a grouped type expression
  void Compiler::visit_type_grouped(const type_expr_grouped_ptr& expr)
  {
    // TODO
  }

  // Visit a generic type expression
  void Compiler::visit_type_generic(const type_expr_generic_ptr& expr)
  {
    // TODO
  }

  // Visit a maybe type expression
  void Compiler::visit_type_maybe(const type_expr_maybe_ptr& expr)
  {
    // TODO
  }

  // Visit an intersection type expression
  void Compiler::visit_type_intersection(const type_expr_intersection_ptr& expr)
  {
    // TODO
  }

  // Visit an union type expression
  void Compiler::visit_type_union(const type_expr_union_ptr& expr)
  {
    // TODO
  }
}
