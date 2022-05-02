#include "ast.h"

namespace dauw
{
  // Accept a visitor on the literal expression
  void ExprLiteral::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_literal(shared_from_this());
  }

  // Accept a visitor on a name expression
  void ExprName::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_name(shared_from_this());
  }

  // Accept a visitor on a parenthesized expression
  void ExprParenthesized::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_parenthesized(shared_from_this());
  }

  // Accept a visitor on a call expression
  void ExprCall::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_call(shared_from_this());
  }

  // Accept a visitor on a get expression
  void ExprSubscript::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_subscript(shared_from_this());
  }

  // Accept a visitor on a get expression
  void ExprGet::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_get(shared_from_this());
  }

  // Accept a visitor on an unary expression
  void ExprUnary::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_unary(shared_from_this());
  }

  // Accept a visitor on a binary expression
  void ExprBinary::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_binary(shared_from_this());
  }

  // Accept a visitor on a block expression
  void ExprBlock::accept(const std::shared_ptr<ExprVisitor>& visitor)
  {
    visitor->visit_block(shared_from_this());
  }
}
