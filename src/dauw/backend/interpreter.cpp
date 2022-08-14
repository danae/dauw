#include "interpreter.hpp"

namespace dauw
{
  // Constructor for the interpreter
  Interpreter::Interpreter(Reporter* reporter)
    : ReporterAware(reporter)
  {
  }

  // Evaluate an expression
  Value Interpreter::evaluate(const expr_ptr& expr)
  {
    expr->accept(shared_from_this());

    if (expr->has_computed_value())
      return expr->computed_value();
    else
      return Value::value_nothing;
  }

  // Evaluate a type expression
  Type Interpreter::evaluate(const type_expr_ptr& expr)
  {
    expr->accept(shared_from_this());
    return expr->type();
  }

  // --------------------------------------------------------------------------
  // EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a literal expression
  void Interpreter::visit_literal(const expr_literal_ptr& expr)
  {
    // Return the value of the literal expression
    expr->set_computed_value(expr->value());
  }

  // Visit a sequence expression
  void Interpreter::visit_sequence(const expr_sequence_ptr& expr)
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
  void Interpreter::visit_record(const expr_record_ptr& expr)
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
  void Interpreter::visit_name(const expr_name_ptr& expr)
  {
    // TODO: Implement evaluating name expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating name expression");
  }

  // Visit a function expression
  void Interpreter::visit_function(const expr_function_ptr& expr)
  {
    // TODO: Implement evaluating function expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating function expression");
  }

  // Visit a function parameter expression
  void Interpreter::visit_function_parameter(const expr_function_parameter_ptr& expr)
  {
    // TODO: Implement evaluating function parameter expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating function parameter expression");
  }

  // Visit a grouped expression
  void Interpreter::visit_grouped(const expr_grouped_ptr& expr)
  {
    // Return the evaluated value of the nested expression
    expr->set_computed_value(evaluate(expr->expr()));
  }

  // Visit a call expression
  void Interpreter::visit_call(const expr_call_ptr& expr)
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
  void Interpreter::visit_get(const expr_get_ptr& expr)
  {
    // TODO: Implement evaluating get expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating get expression");

    /*
    // Compile and validate the object
    evaluate(expr->object());
    */
  }

  // Visit an unary expression
  void Interpreter::visit_unary(const expr_unary_ptr& expr)
  {
    // Check if the expression is a logic not operation
    if (expr->op() == TokenKind::OPERATOR_LOGIC_NOT)
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
      case TokenKind::OPERATOR_SUBTRACT:
        if (expr->check_operand_type(Type::type_int))
          expr->set_computed_value(Value::of_int(-right.as_int()));
        else if (expr->check_operand_type(Type::type_float))
          expr->set_computed_value(Value::of_float(-right.as_float()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for unary operator -");
        break;

      // Length operator
      case TokenKind::OPERATOR_LENGTH:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating length operation");
        break;

      // String operator
      case TokenKind::OPERATOR_STRING:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating string operation");
        break;

      // TODO: Unknown unary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown unary operator");
        break;
    }
  }

  // Visit a binary expression
  void Interpreter::visit_binary(const expr_binary_ptr& expr)
  {
    // Check if the expression is a logic and operation
    if (expr->op() == TokenKind::OPERATOR_LOGIC_AND)
    {
      // TODO: Implement evaluating logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement evaluating logic and operation");
    }

    // TODO: Check if the expression is a logic or expression
    if (expr->op() == TokenKind::OPERATOR_LOGIC_OR)
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
      case TokenKind::OPERATOR_MULTIPLY:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_int(left.as_int() * right.as_int()));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(left.as_float() * right.as_float()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator *");
        break;

      // Divide operator
      case TokenKind::OPERATOR_DIVIDE:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_float(static_cast<dauw_float_t>(left.as_int()) / static_cast<dauw_float_t>(left.as_float())));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(left.as_float() / right.as_float()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator /");
        break;

      // Quotient operator
      case TokenKind::OPERATOR_QUOTIENT:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_int(utils::floordiv(left.as_int(), right.as_int())));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(utils::floordiv(left.as_float(), right.as_float())));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator //");
        break;

      // Remainder operator
      case TokenKind::OPERATOR_REMAINDER:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_int(utils::floormod(left.as_int(), right.as_int())));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(utils::floormod(left.as_float(), right.as_float())));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator %");
        break;
        break;

      // Add operator
      case TokenKind::OPERATOR_ADD:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_int(left.as_int() + right.as_int()));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(left.as_float() + right.as_float()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator +");
        break;

      // Subtract operator
      case TokenKind::OPERATOR_SUBTRACT:
        if (expr->check_operand_type(Type::type_int, Type::type_int))
          expr->set_computed_value(Value::of_int(left.as_int() - right.as_int()));
        else if (expr->check_operand_type(Type::type_float, Type::type_float))
          expr->set_computed_value(Value::of_float(left.as_float() - right.as_float()));
        else
          report<UnimplementedError>(expr->location(), "TODO: Type mismatch for binary operator -");
        break;

      // Range operator
      case TokenKind::OPERATOR_RANGE:
        report<UnimplementedError>(expr->location(), "TODO: Implement evaluating range operation");
        break;

      // Compare operator
      case TokenKind::OPERATOR_COMPARE:
        expr->set_computed_value(Value::of_int(op_compare(expr->location(), left, right)));
        break;

      // Less than operator
      case TokenKind::OPERATOR_LESS:
        expr->set_computed_value(Value::of_bool(op_compare(expr->location(), left, right) < 0));
        break;

      // Less than or equal operator
      case TokenKind::OPERATOR_LESS_EQUAL:
        expr->set_computed_value(Value::of_bool(op_compare(expr->location(), left, right) <= 0));
        break;

      // Greater than operator
      case TokenKind::OPERATOR_GREATER:
        expr->set_computed_value(Value::of_bool(op_compare(expr->location(), left, right) > 0));
        break;

      // Greater than or equal operator
      case TokenKind::OPERATOR_GREATER_EQUAL:
        expr->set_computed_value(Value::of_bool(op_compare(expr->location(), left, right) >= 0));
        break;

      // Match operator
      case TokenKind::OPERATOR_MATCH:
        expr->set_computed_value(Value::of_bool(op_match(expr->location(), left, right)));
        break;

      // Not match operator
      case TokenKind::OPERATOR_NOT_MATCH:
        expr->set_computed_value(Value::of_bool(!op_match(expr->location(), left, right)));
        break;

      // Equal operator
      case TokenKind::OPERATOR_EQUAL:
        expr->set_computed_value(Value::of_bool(op_equals(expr->location(), left, right)));
        break;

      // Not equal operator
      case TokenKind::OPERATOR_NOT_EQUAL:
        expr->set_computed_value(Value::of_bool(!op_equals(expr->location(), left, right)));
        break;

      // Identical operator
      case TokenKind::OPERATOR_IDENTICAL:
        expr->set_computed_value(Value::of_bool(left == right));
        break;

      // Not identical operator
      case TokenKind::OPERATOR_NOT_IDENTICAL:
        expr->set_computed_value(Value::of_bool(left != right));
        break;

      // TODO: Unknown binary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown binary operator");
        break;
    }
  }

  // Visit an echo expression
  void Interpreter::visit_echo(const expr_echo_ptr& expr)
  {
    // Evaluate the nested expression
    auto value = evaluate(expr->expr());

    // Print
    fmt::print("{}\n", value);
  }

  // Visit an if expression
  void Interpreter::visit_if(const expr_if_ptr& expr)
  {
    // TODO: Implement evaluating if expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating if expression");
  }

  // Visit a for expression
  void Interpreter::visit_for(const expr_for_ptr& expr)
  {
    // TODO: Implement evaluating for expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating for expression");
  }

  // Visit a while expression
  void Interpreter::visit_while(const expr_while_ptr& expr)
  {
    // TODO: Implement evaluating while expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating while expression");
  }

  // Visit an until expression
  void Interpreter::visit_until(const expr_until_ptr& expr)
  {
    // TODO: Implement evaluating until expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating until expression");
  }

  // Visit a block expression
  void Interpreter::visit_block(const expr_block_ptr& expr)
  {
    // Iterate over the sub-expressions and evaluate them in order
    for (expr_ptr sub_expr : *expr)
      expr->set_computed_value(evaluate(sub_expr));
  }

  // Visit a def expression
  void Interpreter::visit_def(const expr_def_ptr& expr)
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
  void Interpreter::visit_type_name(const type_expr_name_ptr& expr)
  {
    // TODO: Implement evaluating name type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating name type expression");
  }

  // Visit a grouped type expression
  void Interpreter::visit_type_grouped(const type_expr_grouped_ptr& expr)
  {
    // TODO: Implement evaluating grouped type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating grouped type expression");
  }

  // Visit a generic type expression
  void Interpreter::visit_type_generic(const type_expr_generic_ptr& expr)
  {
    // TODO: Implement evaluating generic type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating generic type expression");
  }

  // Visit a maybe type expression
  void Interpreter::visit_type_maybe(const type_expr_maybe_ptr& expr)
  {
    // TODO: Implement evaluating maybe type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating maybe type expression");
  }

  // Visit an intersection type expression
  void Interpreter::visit_type_intersection(const type_expr_intersection_ptr& expr)
  {
    // TODO: Implement evaluating intersection type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating intersection type expression");
  }

  // Visit an union type expression
  void Interpreter::visit_type_union(const type_expr_union_ptr& expr)
  {
    // TODO: Implement evaluating union type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement evaluating union type expression");
  }

  // --------------------------------------------------------------------------
  // OPERATOR FUNCTIONS
  // --------------------------------------------------------------------------

  // Return the result of comparing two values
  dauw_int_t Interpreter::op_compare(Location& location, Value left, Value right)
  {
    // TODO: Implement comparison of strings
    // TODO: Better type checking
    report<UnimplementedError>(location, "TODO: Implement compare operation function");
    return 0;
  }

  // Return the result of checking if two values match
  dauw_bool_t Interpreter::op_match(Location& location, Value left, Value right)
  {
    report<UnimplementedError>(location, "TODO: Implement match operation function");
    return false;
  }

  // Return the result of checking if two values are equal
  dauw_bool_t Interpreter::op_equals(Location& location, Value left, Value right)
  {
    // First check for reference equality
    if (left == right)
      return true;

    // Otherwise the values are not equal
    return false;
  }
}
