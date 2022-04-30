#include "ast.h"


namespace dauw
{
  // Accept a visitor on the literal expression
  void LiteralExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_literal(this);
  }


  // Accept a visitor on a name expression
  void NameExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_name(this);
  }


  // Accept a visitor on a parenthesized expression
  void ParenthesizedExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_parenthesized(this);
  }


  // Destructor for a call expression
  CallExpr::~CallExpr()
  {
    delete callee_;
    for (auto argument : arguments_)
      delete argument;
  }

  // Accept a visitor on a call expression
  void CallExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_call(this);
  }


  // Destructor for a subscript expression
  SubscriptExpr::~SubscriptExpr()
  {
    delete callee_;
    for (auto argument : arguments_)
      delete argument;
  }

  // Accept a visitor on a get expression
  void SubscriptExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_subscript(this);
  }


  // Destructor for a get expression
  GetExpr::~GetExpr()
  {
    delete callee_;
  }

  // Accept a visitor on a get expression
  void GetExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_get(this);
  }


  // Destructor for an unary expression
  UnaryExpr::~UnaryExpr()
  {
    delete right_;
  }

  // Accept a visitor on an unary expression
  void UnaryExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_unary(this);
  }


  // Destructor for a binary expression
  BinaryExpr::~BinaryExpr()
  {
    delete left_;
    delete right_;
  }

  // Accept a visitor on a binary expression
  void BinaryExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_binary(this);
  }
}
