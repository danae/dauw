#include "ast.hpp"

namespace dauw
{
  // Return the resolved type of the node
  type_ptr& Node::resolved_type()
  {
    return resolved_type_;
  }

  // Return if the node has a resolved type
  bool Node::has_resolved_type()
  {
    return resolved_type_ != nullptr;
  }

  // Set the resolved type of the node
  void Node::set_resolved_type(type_ptr& type)
  {
    resolved_type_ = type;
  }

  // Set the resolved type of the node to that of another node
  void Node::set_resolved_type_from(node_ptr node)
  {
    if (node->has_resolved_type())
      set_resolved_type(node->resolved_type());
  }

  // --------------------------------------------------------------------------

  // Constructor for a literal expression
  ExprLiteral::ExprLiteral(Value value, Location location)
    : value_(value), location_(location)
  {
  }

  // Return the value of the literal expression
  Value ExprLiteral::value()
  {
    return value_;
  }

  // Return the location of the literal expression
  Location& ExprLiteral::location()
  {
    return location_;
  }

  // Accept a visitor on the literal expression
  void ExprLiteral::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_literal(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a sequence expression
  ExprSequence::ExprSequence(Token token, ExprSequence::sequence_type items)
    : token_(token), items_(items)
  {
  }

  // Return the items of the sequence expression
  ExprSequence::sequence_type ExprSequence::items()
  {
    return items_;
  }

  // Iterate over the items of the sequence expression
  ExprSequence::sequence_type::const_iterator ExprSequence::begin()
  {
    return items_.cbegin();
  }
  ExprSequence::sequence_type::const_iterator ExprSequence::end()
  {
    return items_.cend();
  }

  // Return the location of the sequence expression
  Location& ExprSequence::location()
  {
    return token_.location();
  }

  // Accept a visitor on the sequence expression
  void ExprSequence::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_sequence(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a record expression
  ExprRecord::ExprRecord(Token token, ExprRecord::record_type items)
    : token_(token), items_(items)
  {
  }

  // Return the items of the record expression
  ExprRecord::record_type ExprRecord::items()
  {
    return items_;
  }

  // Iterate over the items of the record expression
  ExprRecord::record_type::const_iterator ExprRecord::begin()
  {
    return items_.cbegin();
  }
  ExprRecord::record_type::const_iterator ExprRecord::end()
  {
    return items_.cend();
  }

  // Return the location of the record expression
  Location& ExprRecord::location()
  {
    return token_.location();
  }

  // Accept a visitor on the record expression
  void ExprRecord::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_record(shared_from_this());
  }


  // --------------------------------------------------------------------------

  // Constructor for a name expression
  ExprName::ExprName(Token name)
    : name_(name)
  {
  }

  // Return the name of the name expression
  string_t ExprName::name()
  {
    return name_.value();
  }

  // Return the location of the expression
  Location& ExprName::location()
  {
    return name_.location();
  }

  // Accept a visitor on a name expression
  void ExprName::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_name(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a function definition expression
  ExprFunction::ExprFunction(Token token, ExprFunction::parameters_type parameters, std::optional<type_expr_ptr> return_type, expr_ptr body)
    : token_(token), parameters_(parameters), return_type_(return_type), body_(body)
  {
  }

  // Return the parameters of the function expression
  ExprFunction::parameters_type ExprFunction::parameters()
  {
    return parameters_;
  }

  // Return the return type of the function expression
  type_expr_ptr ExprFunction::return_type()
  {
    return return_type_.value();
  }

  // Return if the function expression has a return type
  bool ExprFunction::has_return_type()
  {
    return return_type_.has_value();
  }

  // Return the body of the function expression
  expr_ptr ExprFunction::body()
  {
    return body_;
  }

  // Return the location of the function expression
  Location& ExprFunction::location()
  {
    return token_.location();
  }

  // Accept a visitor on a function expression
  void ExprFunction::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_function(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a function parameter expression
  ExprFunctionParameter::ExprFunctionParameter(Token name, type_expr_ptr type)
    : name_(name), type_(type)
  {
  }

  // Return the name of the function parameter expression
  string_t ExprFunctionParameter::name()
  {
    return name_.value();
  }

  // Return the type of the function parameter expression
  type_expr_ptr ExprFunctionParameter::type()
  {
    return type_;
  }

  // Return the location of the function parameter expression
  Location& ExprFunctionParameter::location()
  {
    return name_.location();
  }

  // Accept a visitor on a function parameter expression
  void ExprFunctionParameter::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_function_parameter(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a grouped expression
  ExprGrouped::ExprGrouped(expr_ptr expr)
    : expr_(expr)
  {
  }

  // Return the nested expression of the grouped expression
  expr_ptr ExprGrouped::expr()
  {
    return expr_;
  }

  // Return the location of the grouped expression
  Location& ExprGrouped::location()
  {
    return expr_->location();
  }

  // Accept a visitor on a grouped expression
  void ExprGrouped::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_grouped(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a call expression
  ExprCall::ExprCall(expr_ptr callee, Token token, expr_sequence_ptr arguments)
    : callee_(callee), token_(token), arguments_(arguments)
  {
  }

  // Return the callee of the call expression
  expr_ptr ExprCall::callee()
  {
    return callee_;
  }

  // Return the arguments of the call expression
  expr_sequence_ptr ExprCall::arguments()
  {
    return arguments_;
  }

  // Return the location of the call expression
  Location& ExprCall::location()
  {
    return token_.location();
  }

  // Accept a visitor on a call expression
  void ExprCall::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_call(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a get expression
  ExprGet::ExprGet(expr_ptr object, Token name)
    : object_(object), name_(name)
  {
  }

  // Return the object of the get expression
  expr_ptr ExprGet::object()
  {
    return object_;
  }

  // Return the name of the get expression
  string_t ExprGet::name()
  {
    return name_.value();
  }

  // Return the location of the get expression
  Location& ExprGet::location()
  {
    return name_.location();
  }

  // Accept a visitor on a get expression
  void ExprGet::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_get(shared_from_this());
  }


  // --------------------------------------------------------------------------

  // Constructor for an unary expression
  ExprUnary::ExprUnary(Token op, expr_ptr right)
    : op_(op), right_(right)
  {
  }

  // Return the operator of the unary expression
  string_t ExprUnary::op()
  {
    return op_.value();
  }

  // Return the operand of the unary expression
  expr_ptr ExprUnary::right()
  {
    return right_;
  }

  // Return the location of the unary expression
  Location& ExprUnary::location()
  {
    return op_.location();
  }

  // Accept a visitor on an unary expression
  void ExprUnary::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_unary(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a binary expression
  ExprBinary::ExprBinary(expr_ptr left, Token op, expr_ptr right)
    : left_(left), op_(op), right_(right)
  {
  }

  // Return the operator of the binary expression
  string_t ExprBinary::op()
  {
    return op_.value();
  }

  // Return the operands of the binary expression
  expr_ptr ExprBinary::left()
  {
    return left_;
  }
  expr_ptr ExprBinary::right()
  {
    return right_;
  }

  // Return the location of the binary expression
  Location& ExprBinary::location()
  {
    return op_.location();
  }

  // Accept a visitor on a binary expression
  void ExprBinary::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_binary(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for an echo expression
  ExprEcho::ExprEcho(Token keyword, expr_ptr expr)
    : keyword_(keyword), expr_(expr)
  {
  }

  // Return the nested expression of the echo expression
  expr_ptr ExprEcho::expr()
  {
    return expr_;
  }

  // Return the location of the echo expression
  Location& ExprEcho::location()
  {
    return keyword_.location();
  }

  // Accept a visitor on an echo expression
  void ExprEcho::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_echo(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for an if expression
  ExprIf::ExprIf(Token keyword, expr_ptr condition, expr_ptr true_branch, std::optional<expr_ptr> false_branch)
    : keyword_(keyword), condition_(condition), true_branch_(true_branch), false_branch_(false_branch)
  {
  }

  // Return the condition of the if expression
  expr_ptr ExprIf::condition()
  {
    return condition_;
  }

  // Return the true branch of the if expression
  expr_ptr ExprIf::true_branch()
  {
    return true_branch_;
  }

  // Return the false branch of the if expression
  expr_ptr ExprIf::false_branch()
  {
    return false_branch_.value();
  }

  // Return if the if expression has a false branch
  bool ExprIf::has_false_branch()
  {
    return false_branch_.has_value();
  }

  // Return the location of the if expression
  Location& ExprIf::location()
  {
    return keyword_.location();
  }

  // Accept a visitor on a if expression
  void ExprIf::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_if(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a for expression
  ExprFor::ExprFor(Token keyword, Token name, expr_ptr iterable, expr_ptr body)
    : keyword_(keyword), name_(name), iterable_(iterable), body_(body)
  {
  }

  // Return the name of the for expression
  string_t ExprFor::name()
  {
    return name_.value();
  }

  // Return the iterable of the for expression
  expr_ptr ExprFor::iterable()
  {
    return iterable_;
  }

  // Return the body of the for expression
  expr_ptr ExprFor::body()
  {
    return body_;
  }

  // Return the location of the for expression
  Location& ExprFor::location()
  {
    return keyword_.location();
  }

  // Accept a visitor on a for expression
  void ExprFor::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_for(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a while expression
  ExprWhile::ExprWhile(Token keyword, expr_ptr condition, expr_ptr body)
    : keyword_(keyword), condition_(condition), body_(body)
  {
  }

  // Return the condition of the while expression
  expr_ptr ExprWhile::condition()
  {
    return condition_;
  }

  // Return the body of the while expression
  expr_ptr ExprWhile::body()
  {
    return body_;
  }

  // Return the location of the while expression
  Location& ExprWhile::location()
  {
    return keyword_.location();
  }

  // Accept a visitor on a while expression
  void ExprWhile::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_while(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for an until expression
  ExprUntil::ExprUntil(Token keyword, expr_ptr condition, expr_ptr body)
    : keyword_(keyword), condition_(condition), body_(body)
  {
  }

  // Return the condition of the until expression
  expr_ptr ExprUntil::condition()
  {
    return condition_;
  }

  // Return the body of the until expression
  expr_ptr ExprUntil::body()
  {
    return body_;
  }

  // Return the location of the until expression
  Location& ExprUntil::location()
  {
    return keyword_.location();
  }

  // Accept a visitor on an until expression
  void ExprUntil::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_until(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a block expression
  ExprBlock::ExprBlock(ExprBlock::block_type exprs)
    : exprs_(exprs)
  {
  }

  // Return the expressions of the block expression
  ExprBlock::block_type ExprBlock::exprs()
  {
    return exprs_;
  }

  // Iterate over the expressions of the block expression
  ExprBlock::block_type::const_iterator ExprBlock::begin()
  {
    return exprs_.cbegin();
  }
  ExprBlock::block_type::const_iterator ExprBlock::end()
  {
    return exprs_.cend();
  }

  // Return the location of the block expression
  Location& ExprBlock::location()
  {
    // TODO: Possibly use the indent token for the block location
    return exprs_[0]->location();
  }

  // Accept a visitor on a block expression
  void ExprBlock::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_block(shared_from_this());
  }

  // --------------------------------------------------------------------------

  // Constructor for a def expression
  ExprDef::ExprDef(Token name, std::optional<type_expr_ptr> type, expr_ptr value)
    : name_(name), type_(type), value_(value)
  {
  }

  // Return the name of the def expression
  string_t ExprDef::name()
  {
    return name_.value();
  }

  // Return the type of the def expression
  type_expr_ptr ExprDef::type()
  {
    return type_.value();
  }

  // Return if the def expression has a type
  bool ExprDef::has_type()
  {
    return type_.has_value();
  }

  // Return the value of def expression
  expr_ptr ExprDef::value()
  {
    return value_;
  }

  // Return the location of the def expression
  Location& ExprDef::location()
  {
    return name_.location();
  }

  // Accept a visitor on a def expression
  void ExprDef::accept(const expr_visitor_ptr& visitor)
  {
    visitor->visit_def(shared_from_this());
  }

  // --------------------------------------------------------------------------

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
  TypeExprGeneric::TypeExprGeneric(type_expr_ptr base, Token token, TypeExprGeneric::generic_type arguments)
    : base_(base), token_(token), arguments_(arguments)
  {
  }

  // Return the base of the generic type expression
  type_expr_ptr TypeExprGeneric::base()
  {
    return base_;
  }

  // Return the generic arguments of the generic type expression
  TypeExprGeneric::generic_type TypeExprGeneric::arguments()
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
