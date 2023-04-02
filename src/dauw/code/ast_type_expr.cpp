#include "ast_type_expr.hpp"

namespace dauw
{
  // Constructor for a name type expression
  TypeExprName::TypeExprName(Token name)
    : name_(name)
  {
  }

  // Return the name of the name type expression
  string_t TypeExprName::name()
  {
    return name_.value();
  }

  // Return the location of the name type expression
  Location& TypeExprName::location()
  {
    return name_.location();
  }

  // Accept a visitor on a name type expression
  void TypeExprName::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_name(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a grouped type expression
  TypeExprGrouped::TypeExprGrouped(type_expr_ptr expr)
    : expr_(expr)
  {
  }

  // Return the nested type expression of the grouped type expression
  type_expr_ptr TypeExprGrouped::expr()
  {
    return expr_;
  }

  // Return the location of the grouped type expression
  Location& TypeExprGrouped::location()
  {
    return expr_->location();
  }

  // Accept a visitor on a grouped type expression
  void TypeExprGrouped::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_grouped(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a generic type expression
  TypeExprGeneric::TypeExprGeneric(type_expr_ptr base, Token token, std::vector<type_expr_ptr> arguments)
    : base_(base), token_(token), arguments_(arguments)
  {
  }

  // Return the base of the generic type expression
  type_expr_ptr TypeExprGeneric::base()
  {
    return base_;
  }

  // Return the generic arguments of the generic type expression
  std::vector<type_expr_ptr> TypeExprGeneric::arguments()
  {
    return arguments_;
  }

  // Return the location of the generic type expression
  Location& TypeExprGeneric::location()
  {
    return token_.location();
  }

  // Accept a visitor on a generic type expression
  void TypeExprGeneric::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_generic(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a maybe type expression
  TypeExprMaybe::TypeExprMaybe(type_expr_ptr base, Token op)
    : base_(base), op_(op)
  {
  }

  // Return the base of the maybe type expression
  type_expr_ptr TypeExprMaybe::base()
  {
    return base_;
  }

  // Return the location of the maybe type expression
  Location& TypeExprMaybe::location()
  {
    return op_.location();
  }

  // Accept a visitor on a maybe type expression
  void TypeExprMaybe::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_maybe(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for an intersection type expression
  TypeExprIntersection::TypeExprIntersection(type_expr_ptr left, Token op, type_expr_ptr right)
    : left_(left), op_(op), right_(right)
  {
  }

  // Return the operands of the intersection type expression
  type_expr_ptr TypeExprIntersection::left()
  {
    return left_;
  }
  type_expr_ptr TypeExprIntersection::right()
  {
    return right_;
  }

  // Return the location of the intersection type expression
  Location& TypeExprIntersection::location()
  {
    return op_.location();
  }

  // Accept a visitor on an intersection type expression
  void TypeExprIntersection::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_intersection(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for an union type expression
  TypeExprUnion::TypeExprUnion(type_expr_ptr left, Token op, type_expr_ptr right)
    : left_(left), op_(op), right_(right)
  {
  }

  // Return the operands of the union type expression
  type_expr_ptr TypeExprUnion::left()
  {
    return left_;
  }
  type_expr_ptr TypeExprUnion::right()
  {
    return right_;
  }

  // Return the location of the union type expression
  Location& TypeExprUnion::location()
  {
    return op_.location();
  }

  // Accept a visitor on an union type expression
  void TypeExprUnion::accept(const type_expr_visitor_ptr& visitor)
  {
    visitor->visit_type_union(shared_from_this());
  }
}
