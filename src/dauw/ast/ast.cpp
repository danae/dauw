#include "ast.hpp"

namespace dauw
{
  // Return the resolved type of the expression
  Type& Expr::resolved_type()
  {
    return resolved_type_.value();
  }

  // Return if the expression has a resolved type
  bool Expr::has_resolved_type()
  {
    return resolved_type_.has_value();
  }

  // Set the resolved type of the expression
  void Expr::set_resolved_type(Type& type)
  {
    resolved_type_ = std::make_optional(type);
  }

  // Set the resolved type of the expression to that of another expression
  void Expr::set_resolved_type_from(expr_ptr expr)
  {
    if (expr->has_resolved_type())
      set_resolved_type(expr->resolved_type());
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
  ExprName::ExprName(Token name, std::optional<expr_ptr> type)
    : name_(name), type_(type)
  {
  }
  ExprName::ExprName(Token name, expr_ptr type)
    : ExprName(name, std::make_optional(type))
  {
  }
  ExprName::ExprName(Token name)
    : ExprName(name, std::nullopt)
  {
  }

  // Return the name of the name expression
  string_t ExprName::name()
  {
    return name_.value();
  }

  // Return the type of the name expression
  expr_ptr ExprName::type()
  {
    return type_.value();
  }

  // Return if the name expression has a type
  bool ExprName::has_type()
  {
    return type_.has_value();
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
  ExprFunction::ExprFunction(Token token, ExprFunction::parameters_type parameters, std::optional<expr_ptr> return_type, expr_ptr body)
    : token_(token), parameters_(parameters), return_type_(return_type), body_(body)
  {
  }

  // Return the parameters of the function expression
  ExprFunction::parameters_type ExprFunction::parameters()
  {
    return parameters_;
  }

  // Return the return type of the function expression
  expr_ptr ExprFunction::return_type()
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

  // Return the location of the grouped
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
  ExprBlock::ExprBlock(expr_ptr_vector exprs)
    : exprs_(exprs)
  {
  }

  // Return the expressions of the block expression
  expr_ptr_vector ExprBlock::exprs()
  {
    return exprs_;
  }

  // Iterate over the expressions of the block expression
  expr_ptr_vector::const_iterator ExprBlock::begin()
  {
    return exprs_.cbegin();
  }
  expr_ptr_vector::const_iterator ExprBlock::end()
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
  ExprDef::ExprDef(Token name, std::optional<expr_ptr> type, expr_ptr value)
    : name_(name), type_(type), value_(value)
  {
  }

  // Return the name of the def expression
  string_t ExprDef::name()
  {
    return name_.value();
  }

  // Return the type of the def expression
  expr_ptr ExprDef::type()
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
}
