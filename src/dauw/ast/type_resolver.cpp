#include "type_resolver.hpp"

namespace dauw
{
  // Resolve the type of an expression
  void TypeResolver::resolve(const expr_ptr& expr, bool throw_if_failed)
  {
    // Accept this visitor on the expression
    expr->accept(shared_from_this());

    // Check if the type of the expression has been resolved
    if (throw_if_failed && !expr->has_resolved_type())
      throw RuntimeError(expr->location(), "Could not infer the type of the expression");
  }

  // Resolve if a type is a subtype of another type
  void TypeResolver::resolve_subtype_of(const expr_ptr& expr, Type& type)
  {
    // TODO: Implement proper subtype check
    if (expr->resolved_type() != type)
      throw RuntimeError(expr->location(), fmt::format("Expected a subtype of {}, but found {}", expr->resolved_type().name(), type.name()));
  }

  // Visit a literal expression
  void TypeResolver::visit_literal(const expr_literal_ptr& expr)
  {
    // The type of the literal expression is the type of its value
    expr->set_resolved_type(expr->value().type());
  }

  // Visit a sequence expression
  void TypeResolver::visit_sequence(const expr_sequence_ptr& expr)
  {
    // Resolve the items of the sequence expression
    for (auto item : *expr)
      resolve(item);

    // TODO: Implement type resolving for sequence expressions
  }

  // Visit a record expression
  void TypeResolver::visit_record(const expr_record_ptr& expr)
  {
    // Resolve the items of the record expression
    for (auto item : *expr)
      resolve(std::get<1>(item));

    // TODO: Implement type resolving for record expressions
  }

  // Visit a name expression
  void TypeResolver::visit_name(const expr_name_ptr& expr)
  {
    // Resolve the type of the name expression
    if (expr->has_type())
      resolve(expr->type());

    // TODO: Implement type resolving for name expressions
  }

  // Visit a function expression
  void TypeResolver::visit_function(const expr_function_ptr& expr)
  {
    // Resolve the body of the function expression
    resolve(expr->body());

    // TODO: Check if the resolved type is a subtype of the defined return type
    /*if (expr->has_return_type())
      resolve_subtype_of(expr->body(), expr->return_type());*/

    // The type of the function declaration expression is thate of its value
    expr->set_resolved_type_from(expr->body());
  }

  // Visit a grouped expression
  void TypeResolver::visit_grouped(const expr_grouped_ptr& expr)
  {
    // Resolve the nested expression
    resolve(expr->expr());

    // The type of the grouped expression is that of its nested expression
    expr->set_resolved_type_from(expr->expr());
  }

  // Visit a call expression
  void TypeResolver::visit_call(const expr_call_ptr& expr)
  {
    // Resolve the callee of the call expression
    resolve(expr->callee());

    // Resolve the arguments of the call expression
    resolve(expr->arguments());

    // TODO: Implement type resolving for call expressions
  }

  // Visit a get expression
  void TypeResolver::visit_get(const expr_get_ptr& expr)
  {
    // Resolve the object of the get expression
    resolve(expr->object());

    // TODO: Implement type resolving for get expressions
  }

  // Visit an unary expression
  void TypeResolver::visit_unary(const expr_unary_ptr& expr)
  {
    // Resolve the operand of the unary expression
    resolve(expr->right());

    // TODO: Implement type resolving for unary expressions
  }

  // Visit a binary expression
  void TypeResolver::visit_binary(const expr_binary_ptr& expr)
  {
    // Resolve the operands of the binary expression
    resolve(expr->left());
    resolve(expr->right());

    // TODO: Implement type resolving for binary expressions
  }

  // Visit an if expression
  void TypeResolver::visit_if(const expr_if_ptr& expr)
  {
    // Resolve the condition of the if expression
    resolve(expr->condition());

    // Resolve the true branch of the if expression
    resolve(expr->true_branch());

    // Resolve the false branch of the if expression
    if (expr->has_false_branch())
      resolve(expr->false_branch());

    // TODO: Implement type resolving for if expressions
  }

  // Visit a for expression
  void TypeResolver::visit_for(const expr_for_ptr& expr)
  {

  }

  // Visit a while expression
  void TypeResolver::visit_while(const expr_while_ptr& expr)
  {

  }

  // Visit an until expression
  void TypeResolver::visit_until(const expr_until_ptr& expr)
  {

  }

  // Visit a block expression
  void TypeResolver::visit_block(const expr_block_ptr& expr)
  {
    // Resolve the expressions of the block expression
    for (auto sub_expr : *expr)
      resolve(sub_expr);

    // The type of the block expression is that of the last expression in the block
    if (expr->exprs().size() > 0)
      expr->set_resolved_type_from(expr->exprs().back());
  }

  // Visit a def expression
  void TypeResolver::visit_def(const expr_def_ptr& expr)
  {
    // Resolve the value of the def expression
    resolve(expr->value());

    // TODO: Check if the resolved type is a subtype of the defined type
    /*if (expr->has_type())
      resolve_subtype_of(expr->value(), expr->type());*/

    // The type of the def expression is that of its value
    expr->set_resolved_type_from(expr->value());
  }
}
