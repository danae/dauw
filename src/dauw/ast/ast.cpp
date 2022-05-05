#include "ast.hpp"

namespace dauw
{
  // Constructor for a literal expression
  ExprLiteral::ExprLiteral(Value value, Location location) : value_(value), location_(location)
  {
  }

  // Return the location of the literal expression
  Location& ExprLiteral::location()
  {
    return location_;
  }

  // Accept a visitor on the literal expression
  void ExprLiteral::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_literal(shared_from_this());
  }


  // Constructor for a name expression
  ExprName::ExprName(Token name) : name_(name)
  {
  }

  // Return the location of the expression
  Location& ExprName::location()
  {
    return name_.location();
  }

  // Accept a visitor on a name expression
  void ExprName::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_name(shared_from_this());
  }


  // Constructor for a parenthesized expression
  ExprParenthesized::ExprParenthesized(std::shared_ptr<Expr> expr)
  {
    expr_ = expr;
  }

  // Return the location of the parenthesized
  Location& ExprParenthesized::location()
  {
    return expr_->location();
  }

  // Accept a visitor on a parenthesized expression
  void ExprParenthesized::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_parenthesized(shared_from_this());
  }


  // Constructor for a call expression
  ExprCall::ExprCall(std::shared_ptr<Expr> callee, Token end, std::vector<std::shared_ptr<Expr>> arguments)
  {
    callee_ = callee;
    end_ = end;
    arguments_ = arguments;
  }

  // Return the location of the call expression
  Location& ExprCall::location()
  {
    return end_.location();
  }

  // Accept a visitor on a call expression
  void ExprCall::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_call(shared_from_this());
  }


  // Constructor for a subscript expression
  ExprSubscript::ExprSubscript(std::shared_ptr<Expr> indexee, Token end, std::vector<std::shared_ptr<Expr>> arguments)
  {
    indexee_ = indexee;
    end_ = end;
    arguments_ = arguments;
  }

  // Return the location of the subscript expression
  Location& ExprSubscript::location()
  {
    return end_.location();
  }

  // Accept a visitor on a subscript expression
  void ExprSubscript::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_subscript(shared_from_this());
  }


  // Constructor for a get expression
  ExprGet::ExprGet(std::shared_ptr<Expr> object, Token name)
  {
    object_ = object;
    name_ = name;
  }

  // Return the location of the get expression
  Location& ExprGet::location()
  {
    return name_.location();
  }

  // Accept a visitor on a get expression
  void ExprGet::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_get(shared_from_this());
  }


  // Constructor for an unary expression
  ExprUnary::ExprUnary(Token op, std::shared_ptr<Expr> right)
  {
    op_ = op;
    right_ = right;
  }

  // Return the location of the unary expression
  Location& ExprUnary::location()
  {
    return op_.location();
  }

  // Accept a visitor on an unary expression
  void ExprUnary::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_unary(shared_from_this());
  }


  // Constructor for a binary expression
  ExprBinary::ExprBinary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
  {
    left_ = left;
    op_ = op;
    right_ = right;
  }

  // Return the location of the binary expression
  Location& ExprBinary::location()
  {
    return op_.location();
  }

  // Accept a visitor on a binary expression
  void ExprBinary::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_binary(shared_from_this());
  }


  // Constructor for a block expression
  ExprBlock::ExprBlock(std::vector<std::shared_ptr<Expr>> exprs)
  {
    exprs_ = exprs;
  }

  // Return the location of the block expression
  Location& ExprBlock::location()
  {
    // TODO: Figure out a better way to get the location of a block
    return exprs_[0]->location();
  }

  // Accept a visitor on a block expression
  void ExprBlock::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_block(shared_from_this());
  }
}
