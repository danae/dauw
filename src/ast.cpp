#include "ast.h"


namespace dauw
{
  // Accept a visitor on the literal expression
  void LiteralExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_literal(this);
  }


  // Destructor for a nested expression
  NestedExpr::~NestedExpr()
  {
    delete left_;
    delete right_;
  }

  // Accept a visitor on the name expression
  void NestedExpr::accept(ExprVisitor& visitor)
  {
    visitor.visit_nested(this);
  }
}
