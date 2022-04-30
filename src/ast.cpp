#include "ast.h"

//-----------------------------------------------------------------------------

// Constructor for an expression
dauw::expr::expr(Location& location)
  : location(location)
{
}

//-----------------------------------------------------------------------------

// Constructor for a literal expression
dauw::expr_literal::expr_literal(Location& location, Value value)
  : expr(location), value(value)
{
}

// Accept a visitor on the literal expression
void dauw::expr_literal::accept_visitor(expr_visitor& visitor)
{
  visitor.visit_literal(*this);
}

// Accept an evaluator on the literal expression
dauw::Object dauw::expr_literal::accept_evaluator(expr_evaluator& evaluator)
{
  return evaluator.evaluate_literal(*this);
}

//-----------------------------------------------------------------------------

// Constructor for a name expression
dauw::expr_name::expr_name(Location& location, std::string name)
  : expr(location), name(name)
{
}

// Accept a visitor on the name expression
void dauw::expr_name::accept_visitor(expr_visitor& visitor)
{
  visitor.visit_name(*this);
}

// Accept an evaluator on the name expression
dauw::Object dauw::expr_name::accept_evaluator(expr_evaluator& evaluator)
{
  return evaluator.evaluate_name(*this);
}

//-----------------------------------------------------------------------------
