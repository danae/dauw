#include "type_resolver.hpp"

namespace dauw::backend
{
  // Constructor for the type resolver
  TypeResolver::TypeResolver(Reporter* reporter)
    : ReporterAware(reporter)
  {
  }

  // Resolve the type of an expression
  void TypeResolver::resolve(const ast::expr_ptr& expr, bool throw_if_failed)
  {
    // Accept this visitor on the expression
    expr->accept(shared_from_this());

    // Check if the type of the expression has been resolved
    if (throw_if_failed && !expr->has_type())
      report<TypeUnresolvedError>(expr->location(), "Could not infer the type of the expression");
  }

  // Resolve if a type is a subtype of another type
  void TypeResolver::resolve_subtype_of(const ast::expr_ptr& expr, internals::Type& type)
  {
    // TODO: Implement proper subtype check
    if (expr->type() != type)
      report<TypeMismatchError>(expr->location(), fmt::format("Expected a subtype of {}, but found {}", expr->type().name(), type.name()));
  }

  // --------------------------------------------------------------------------
  // EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a literal expression
  void TypeResolver::visit_literal(const ast::expr_literal_ptr& expr)
  {
    // The type of the literal expression is the type of its value
    expr->set_type(expr->value().type());
  }

  // Visit a sequence expression
  void TypeResolver::visit_sequence(const ast::expr_sequence_ptr& expr)
  {
    // Resolve the items of the sequence expression
    for (auto item_expr : *expr)
      resolve(item_expr);

    // TODO: Implement generic resolving of sequence expression
    expr->set_type(internals::Type::type_sequence);
  }

  // Visit a record expression
  void TypeResolver::visit_record(const ast::expr_record_ptr& expr)
  {
    // Resolve the items of the record expression
    for (auto item_expr : *expr)
      resolve(std::get<1>(item_expr));

    // TODO: Implement generic resolving of record expressions
    expr->set_type(internals::Type::type_record);
  }

  // Visit a name expression
  void TypeResolver::visit_name(const ast::expr_name_ptr& expr)
  {
    // TODO: Implement resolving name expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving name expression");
  }

  // Visit a function expression
  void TypeResolver::visit_function(const ast::expr_function_ptr& expr)
  {
    // Resolve the body of the function expression
    resolve(expr->body());

    // TODO: Check if the resolved type is a subtype of the defined return type

    // The type of the function declaration expression is that of its value
    expr->set_type(internals::Type::type_function);
  }

  // Visit a function parameter expression
  void TypeResolver::visit_function_parameter(const ast::expr_function_parameter_ptr& expr)
  {
    // TODO: Implement resolving function parameter expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving function parameter expression");
  }

  // Visit a grouped expression
  void TypeResolver::visit_grouped(const ast::expr_grouped_ptr& expr)
  {
    // Resolve the nested expression of the grouped expression
    resolve(expr->expr());

    // The type of the grouped expression is that of its nested expression
    expr->set_type_from(expr->expr());
  }

  // Visit a call expression
  void TypeResolver::visit_call(const ast::expr_call_ptr& expr)
  {
    // Resolve the callee of the call expression
    resolve(expr->callee());

    // Resolve the arguments of the call expression
    resolve(expr->arguments());

    // TODO: Implement resolving call expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving call expression");
  }

  // Visit a get expression
  void TypeResolver::visit_get(const ast::expr_get_ptr& expr)
  {
    // Resolve the object of the get expression
    resolve(expr->object());

    // TODO: Implement resolving get expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving get expression");
  }

  // Visit an unary expression
  void TypeResolver::visit_unary(const ast::expr_unary_ptr& expr)
  {
    // Check if the expression is a logic not operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_NOT)
    {
      // TODO: Implement resolving logic not operation
      report<UnimplementedError>(expr->location(), "TODO: Implement resolving logic not operation");
    }

    // Resolve the operand of the unary expression
    resolve(expr->right());

    // Check and resolve the operator
    switch (expr->op())
    {
      // Known unary operators
      case frontend::TokenKind::OPERATOR_SUBTRACT:
        visit_unary_negate(expr);
        break;

      case frontend::TokenKind::OPERATOR_LENGTH:
        expr->set_type(internals::Type::type_int);
        break;

      case frontend::TokenKind::OPERATOR_STRING:
        expr->set_type(internals::Type::type_string);
        break;

      // TODO: Unknown unary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown unary operator");
        break;
    }
  }

  // Visit a binary expression
  void TypeResolver::visit_binary(const ast::expr_binary_ptr& expr)
  {
    // Check if the expression is a logic and operation
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_AND)
    {
      // TODO: Implement compiling logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement compiling logic and operation");
    }

    // TODO: Check if the expression is a logic or expression
    if (expr->op() == frontend::TokenKind::OPERATOR_LOGIC_OR)
    {
      // TODO: Implement compiling logic and operation
      report<UnimplementedError>(expr->location(), "TODO: Implement compiling logic or operation");
    }

    // Resolve the operands of the binary expression
    resolve(expr->left());
    resolve(expr->right());

    // Check and resolve the operator
    switch (expr->op())
    {
      case frontend::TokenKind::OPERATOR_MULTIPLY:
        visit_binary_multiply(expr);
        break;

      case frontend::TokenKind::OPERATOR_DIVIDE:
        visit_binary_divide(expr);
        break;

      case frontend::TokenKind::OPERATOR_QUOTIENT:
        visit_binary_quotient(expr);
        break;

      case frontend::TokenKind::OPERATOR_REMAINDER:
        visit_binary_remainder(expr);
        break;

      case frontend::TokenKind::OPERATOR_ADD:
        visit_binary_add(expr);
        break;

      case frontend::TokenKind::OPERATOR_SUBTRACT:
        visit_binary_subtract(expr);
        break;

      case frontend::TokenKind::OPERATOR_RANGE:
        visit_binary_range(expr);
        break;

      case frontend::TokenKind::OPERATOR_COMPARE:
        expr->set_type(internals::Type::type_int);
        break;

      case frontend::TokenKind::OPERATOR_LESS:
      case frontend::TokenKind::OPERATOR_LESS_EQUAL:
      case frontend::TokenKind::OPERATOR_GREATER:
      case frontend::TokenKind::OPERATOR_GREATER_EQUAL:
      case frontend::TokenKind::OPERATOR_MATCH:
      case frontend::TokenKind::OPERATOR_NOT_MATCH:
      case frontend::TokenKind::OPERATOR_EQUAL:
      case frontend::TokenKind::OPERATOR_NOT_EQUAL:
      case frontend::TokenKind::OPERATOR_IDENTICAL:
      case frontend::TokenKind::OPERATOR_NOT_IDENTICAL:
        expr->set_type(internals::Type::type_bool);
        break;

      // TODO: Unknown binary operator
      default:
        report<UnimplementedError>(expr->location(), "TODO: Unknown binary operator");
        break;
    }
  }

  // Visit an echo expression
  void TypeResolver::visit_echo(const ast::expr_echo_ptr& expr)
  {
    // Resolve the nested expression of the echo expression
    resolve(expr->expr());

    // TODO: Implement resolving echo expression
    expr->set_type(internals::Type::type_nothing);
  }

  // Visit an if expression
  void TypeResolver::visit_if(const ast::expr_if_ptr& expr)
  {
    // Resolve the condition of the if expression
    resolve(expr->condition());

    // Resolve the true branch of the if expression
    resolve(expr->true_branch());

    // Resolve the false branch of the if expression
    if (expr->has_false_branch())
      resolve(expr->false_branch());

    // TODO: Implement resolving if expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving if expression");
  }

  // Visit a for expression
  void TypeResolver::visit_for(const ast::expr_for_ptr& expr)
  {
    // Resolve the iterable of the for expression
    resolve(expr->iterable());

    // Resolve the body of the for expression
    resolve(expr->body());

    // TODO: Implement resolving for expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving for expression");
  }

  // Visit a while expression
  void TypeResolver::visit_while(const ast::expr_while_ptr& expr)
  {
    // Resolve the condition of the while expression
    resolve(expr->condition());

    // Resolve the body of the while expression
    resolve(expr->body());

    // TODO: Implement resolving while expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving while expression");
  }

  // Visit an until expression
  void TypeResolver::visit_until(const ast::expr_until_ptr& expr)
  {
    // Resolve the condition of the while expression
    resolve(expr->condition());

    // Resolve the body of the while expression
    resolve(expr->body());

    // TODO: Implement resolving until expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving until expression");
  }

  // Visit a block expression
  void TypeResolver::visit_block(const ast::expr_block_ptr& expr)
  {
    // Resolve the expressions of the block expression
    for (auto sub_expr : *expr)
      resolve(sub_expr);

    // The type of the block expression is that of the last expression in the block
    if (expr->exprs().size() > 0)
      expr->set_type_from(expr->exprs().back());
  }

  // Visit a def expression
  void TypeResolver::visit_def(const ast::expr_def_ptr& expr)
  {
    // Resolve the value of the def expression
    resolve(expr->value());

    // TODO: Check if the resolved type is a subtype of the defined type

    // The type of the def expression is that of its value
    expr->set_type_from(expr->value());
  }

  // --------------------------------------------------------------------------
  // TYPE EXPRESSION VISITOR IMPLEMENTATION
  // --------------------------------------------------------------------------

  // Visit a name type expression
  void TypeResolver::visit_type_name(const ast::type_expr_name_ptr& expr)
  {
    // TODO: Implement resolving name type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving name type expression");
  }

  // Visit a grouped type expression
  void TypeResolver::visit_type_grouped(const ast::type_expr_grouped_ptr& expr)
  {
    // TODO: Implement resolving grouped type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving grouped type expression");
  }

  // Visit a generic type expression
  void TypeResolver::visit_type_generic(const ast::type_expr_generic_ptr& expr)
  {
    // TODO: Implement resolving generic type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving generic type expression");
  }

  // Visit a maybe type expression
  void TypeResolver::visit_type_maybe(const ast::type_expr_maybe_ptr& expr)
  {
    // TODO: Implement resolving maybe type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving maybe type expression");
  }

  // Visit an intersection type expression
  void TypeResolver::visit_type_intersection(const ast::type_expr_intersection_ptr& expr)
  {
    // TODO: Implement resolving intersection type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving intersection type expression");
  }

  // Visit an union type expression
  void TypeResolver::visit_type_union(const ast::type_expr_union_ptr& expr)
  {
    // TODO: Implement resolving union type expression
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving union type expression");
  }

  // --------------------------------------------------------------------------
  // HELPER FUNCTIONS FOR OPERATORS
  // --------------------------------------------------------------------------

  // Resolve the negate operator
  void TypeResolver::visit_unary_negate(const ast::expr_unary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the multiply operator
  void TypeResolver::visit_binary_multiply(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the divide operator
  void TypeResolver::visit_binary_divide(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_real);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the quotient operator
  void TypeResolver::visit_binary_quotient(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the remainder operator
  void TypeResolver::visit_binary_remainder(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the add operator
  void TypeResolver::visit_binary_add(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
    else if (expr->check_operand_type(internals::Type::type_string, internals::Type::type_string))
      expr->set_type(internals::Type::type_string);
  }

  // Resolve the subtract operator
  void TypeResolver::visit_binary_subtract(const ast::expr_binary_ptr& expr)
  {
    if (expr->check_operand_type(internals::Type::type_int, internals::Type::type_int))
      expr->set_type(internals::Type::type_int);
    else if (expr->check_operand_type(internals::Type::type_real, internals::Type::type_real))
      expr->set_type(internals::Type::type_real);
  }

  // Resolve the range operator
  void TypeResolver::visit_binary_range(const ast::expr_binary_ptr& expr)
  {
    // TODO: Implement resolving range operator
    report<UnimplementedError>(expr->location(), "TODO: Implement resolving range operator");
  }
}
