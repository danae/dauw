#include "interpreter.hpp"

namespace dauw::backend
{
  // Constructor for the interpreter
  Interpreter::Interpreter(Reporter* reporter)
    : ReporterAware(reporter)
  {
  }

  // Evaluate an expression
  internals::Value Interpreter::evaluate(const ast::expr_ptr& expr)
  {
    expr->accept(shared_from_this());

    if (expr->has_computed_value())
      return expr->computed_value();
    else
      return internals::Value::value_nothing;
  }

  // Evaluate a type expression
  internals::Type Interpreter::evaluate(const ast::type_expr_ptr& expr)
  {
    expr->accept(shared_from_this());
    return expr->type();
  }

  // --------------------------------------------------------------------------
  // EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a literal expression
  void Interpreter::visit_literal(const ast::expr_literal_ptr& expr)
  {
    // Return the value of the literal expression
    expr->set_computed_value(expr->value());
  }

  // Visit a sequence expression
  void Interpreter::visit_sequence(const ast::expr_sequence_ptr& expr)
  {
    // TODO: Implement evaluating sequence expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating sequence expression");

    /*
    // Compile the items of the sequence expression
    for (auto item_expr : *expr)
      evaluate(item_expr);
    */
  }

  // Visit a record expression
  void Interpreter::visit_record(const ast::expr_record_ptr& expr)
  {
    // TODO: Implement evaluating record expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating record expression");

    /*
    // Compile the items of the record expression
    for (auto item_expr : *expr)
      evaluate(std::get<1>(item_expr));
    */
  }

  // Visit a name expression
  void Interpreter::visit_name(const ast::expr_name_ptr& expr)
  {
    // TODO: Implement evaluating name expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating name expression");
  }

  // Visit a function expression
  void Interpreter::visit_function(const ast::expr_function_ptr& expr)
  {
    // TODO: Implement evaluating function expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating function expression");
  }

  // Visit a function parameter expression
  void Interpreter::visit_function_parameter(const ast::expr_function_parameter_ptr& expr)
  {
    // TODO: Implement evaluating function parameter expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating function parameter expression");
  }

  // Visit a grouped expression
  void Interpreter::visit_grouped(const ast::expr_grouped_ptr& expr)
  {
    // Return the evaluated value of the nested expression
    expr->set_computed_value(evaluate(expr->expr()));
  }

  // Visit a call expression
  void Interpreter::visit_call(const ast::expr_call_ptr& expr)
  {
    // TODO: Implement evaluating call expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating call expression");

    /*
    // Compile and validate the callee
    evaluate(expr->callee());

    // Compile and validate the arguments
    evaluate(expr->arguments());
    */
  }

  // Visit a get expression
  void Interpreter::visit_get(const ast::expr_get_ptr& expr)
  {
    // TODO: Implement evaluating get expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating get expression");

    /*
    // Compile and validate the object
    evaluate(expr->object());
    */
  }

  // Visit an unary expression
  void Interpreter::visit_unary(const ast::expr_unary_ptr& expr)
  {
    // Check if the expression is a logic not operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_NOT)
    {
      // TODO: Implement evaluating logic not operation
      report<UnimplementedError>(expr->location(), "TODO: Implement evaluating logic not operation");
    }

    // Evaluate the operand of the unary expression
    auto right = evaluate(expr->right());

    // Check and evaluate the operator
    // TODO: Implement operator overloading
    switch (expr->op())
    {
      // Negate operator
      case frontend::TokenKind::OPERATOR_SUBTRACT:
        if (expr->check_operand_type(internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(-right.as_int()));
        else if (expr->check_operand_type(internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(-right.as_real()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for unary operator -");
        break;

      // Length operator
      case frontend::TokenKind::OPERATOR_LENGTH:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating length operation");
        break;

      // String operator
      case frontend::TokenKind::OPERATOR_STRING:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating string operation");
        break;

      // TODO: Unknown unary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown unary operator");
        break;
    }
  }

  // Visit a binary expression
  void Interpreter::visit_binary(const ast::expr_binary_ptr& expr)
  {
    // Check if the expression is a logic and operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_AND)
    {
      // TODO: Implement evaluating logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement evaluating logic and operation");
    }

    // TODO: Check if the expression is a logic or expression
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_OR)
    {
      // TODO: Implement evaluating logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement evaluating logic or operation");
    }

    // Evaluate the operands of the binary expression
    auto left = evaluate(expr->left());
    auto right = evaluate(expr->right());

    // Check and evaluate the operator
    // TODO: Implement operator overloading
    switch (expr->op())
    {
      // Multiply operator
      case frontend::TokenKind::OPERATOR_MULTIPLY:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(left.as_int() * right.as_int()));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(left.as_real() * right.as_real()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator *");
        break;

      // Divide operator
      case frontend::TokenKind::OPERATOR_DIVIDE:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_real(static_cast<dauw_real_t>(left.as_int()) / static_cast<dauw_real_t>(left.as_real())));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(left.as_real() / right.as_real()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator /");
        break;

      // Quotient operator
      case frontend::TokenKind::OPERATOR_QUOTIENT:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(utils::floordiv(left.as_int(), right.as_int())));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(utils::floordiv(left.as_real(), right.as_real())));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator //");
        break;

      // Remainder operator
      case frontend::TokenKind::OPERATOR_REMAINDER:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(utils::floormod(left.as_int(), right.as_int())));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(utils::floormod(left.as_real(), right.as_real())));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator %");
        break;
        break;

      // Add operator
      case frontend::TokenKind::OPERATOR_ADD:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(left.as_int() + right.as_int()));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(left.as_real() + right.as_real()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator +");
        break;

      // Subtract operator
      case frontend::TokenKind::OPERATOR_SUBTRACT:
        if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
          expr->set_computed_value(internals::Value::of_int(left.as_int() - right.as_int()));
        else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
          expr->set_computed_value(internals::Value::of_real(left.as_real() - right.as_real()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator -");
        break;

      // Range operator
      case frontend::TokenKind::OPERATOR_RANGE:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating range operation");
        break;

      // Compare operator
      case frontend::TokenKind::OPERATOR_COMPARE:
        expr->set_computed_value(internals::Value::of_int(op_compare(expr->location(), left, right)));
        break;

      // Less than operator
      case frontend::TokenKind::OPERATOR_LESS:
        expr->set_computed_value(internals::Value::of_bool(op_compare(expr->location(), left, right) < 0));
        break;

      // Less than or equal operator
      case frontend::TokenKind::OPERATOR_LESS_EQUAL:
        expr->set_computed_value(internals::Value::of_bool(op_compare(expr->location(), left, right) <= 0));
        break;

      // Greater than operator
      case frontend::TokenKind::OPERATOR_GREATER:
        expr->set_computed_value(internals::Value::of_bool(op_compare(expr->location(), left, right) > 0));
        break;

      // Greater than or equal operator
      case frontend::TokenKind::OPERATOR_GREATER_EQUAL:
        expr->set_computed_value(internals::Value::of_bool(op_compare(expr->location(), left, right) >= 0));
        break;

      // Match operator
      case frontend::TokenKind::OPERATOR_MATCH:
        expr->set_computed_value(internals::Value::of_bool(op_match(expr->location(), left, right)));
        break;

      // Not match operator
      case frontend::TokenKind::OPERATOR_NOT_MATCH:
        expr->set_computed_value(internals::Value::of_bool(!op_match(expr->location(), left, right)));
        break;

      // Equal operator
      case frontend::TokenKind::OPERATOR_EQUAL:
        expr->set_computed_value(internals::Value::of_bool(op_equals(expr->location(), left, right)));
        break;

      // Not equal operator
      case frontend::TokenKind::OPERATOR_NOT_EQUAL:
        expr->set_computed_value(internals::Value::of_bool(!op_equals(expr->location(), left, right)));
        break;

      // Identical operator
      case frontend::TokenKind::OPERATOR_IDENTICAL:
        expr->set_computed_value(left == right);
        break;

      // Not identical operator
      case frontend::TokenKind::OPERATOR_NOT_IDENTICAL:
        expr->set_computed_value(left != right);
        break;

      // TODO: Unknown binary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown binary operator");
        break;
    }
  }

  // Visit an echo expression
  void Interpreter::visit_echo(const ast::expr_echo_ptr& expr)
  {
    // Evaluate the nested expression
    auto value = evaluate(expr->expr());

    // Print
    fmt::print("{}\n", value);
  }

  // Visit an if expression
  void Interpreter::visit_if(const ast::expr_if_ptr& expr)
  {
    // TODO: Implement evaluating if expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating if expression");
  }

  // Visit a for expression
  void Interpreter::visit_for(const ast::expr_for_ptr& expr)
  {
    // TODO: Implement evaluating for expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating for expression");
  }

  // Visit a while expression
  void Interpreter::visit_while(const ast::expr_while_ptr& expr)
  {
    // TODO: Implement evaluating while expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating while expression");
  }

  // Visit an until expression
  void Interpreter::visit_until(const ast::expr_until_ptr& expr)
  {
    // TODO: Implement evaluating until expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating until expression");
  }

  // Visit a block expression
  void Interpreter::visit_block(const ast::expr_block_ptr& expr)
  {
    // Iterate over the sub-expressions and evaluate them in order
    for (ast::expr_ptr sub_expr : *expr)
      expr->set_computed_value(evaluate(sub_expr));
  }

  // Visit a def expression
  void Interpreter::visit_def(const ast::expr_def_ptr& expr)
  {
    // TODO: Implement evaluating def expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating def expression");

    /*
    // Compile the value
    evaluate(expr->value());
    */
  }

  // --------------------------------------------------------------------------
  // TYPE EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a name type expression
  void Interpreter::visit_type_name(const ast::type_expr_name_ptr& expr)
  {
    // TODO: Implement evaluating name type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating name type expression");
  }

  // Visit a grouped type expression
  void Interpreter::visit_type_grouped(const ast::type_expr_grouped_ptr& expr)
  {
    // TODO: Implement evaluating grouped type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating grouped type expression");
  }

  // Visit a generic type expression
  void Interpreter::visit_type_generic(const ast::type_expr_generic_ptr& expr)
  {
    // TODO: Implement evaluating generic type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating generic type expression");
  }

  // Visit a maybe type expression
  void Interpreter::visit_type_maybe(const ast::type_expr_maybe_ptr& expr)
  {
    // TODO: Implement evaluating maybe type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating maybe type expression");
  }

  // Visit an intersection type expression
  void Interpreter::visit_type_intersection(const ast::type_expr_intersection_ptr& expr)
  {
    // TODO: Implement evaluating intersection type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating intersection type expression");
  }

  // Visit an union type expression
  void Interpreter::visit_type_union(const ast::type_expr_union_ptr& expr)
  {
    // TODO: Implement evaluating union type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating union type expression");
  }

  // --------------------------------------------------------------------------
  // OPERATOR FUNCTIONS
  // --------------------------------------------------------------------------

  // Return the result of comparing two values
  dauw_int_t Interpreter::op_compare(frontend::Location& location, internals::Value left, internals::Value right)
  {
    // TODO: Implement comparison of strings
    // TODO: Better type checking
    report<UnimplementedError>(location, "TODO: Implement compare operation function");
    return 0;
  }

  // Return the result of checking if two values match
  dauw_bool_t Interpreter::op_match(frontend::Location& location, internals::Value left, internals::Value right)
  {
    report<UnimplementedError>(location, "TODO: Implement match operation function");
    return false;
  }

  // Return the result of checking if two values are equal
  dauw_bool_t Interpreter::op_equals(frontend::Location& location, internals::Value left, internals::Value right)
  {
    // First check for reference equality
    if (left == right)
      return true;

    // Otherwise the values are not equal
    return false;
  }
}
