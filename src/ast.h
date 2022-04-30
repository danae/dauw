#pragma once

#include <string>

#include "common.h"
#include "object.h"
#include "source_location.h"


namespace dauw
{
  // Forward declarations for visitor and evaluator
  class expr_visitor;
  class expr_evaluator;


  // Base class for an expression
  class expr
  {
  public:
    // The Location of the expression
    Location& location;


    // Constructor for an expression
    expr(Location& location);

    // Accept a visitor or evaluator on the expression
    virtual void accept_visitor(expr_visitor& visitor) = 0;
    virtual Object accept_evaluator(expr_evaluator& evaluator) = 0;
  };


  // Expression for a literal
  class expr_literal : public expr
  {
  public:
    // The value of the literal
    Value value;


    // Constructor for a literal expression
    expr_literal(Location& location, Value value);

    // Accept a visitor or evaluator on the literal expression
    void accept_visitor(expr_visitor& visitor) override;
    Object accept_evaluator(expr_evaluator& evaluator) override;
  };


  // Expression for a name
  class expr_name : public expr
  {
  public:
    // The name of the literal
    std::string name;


    // Constructor for a name expression
    expr_name(Location& location, std::string name);

    // Accept a visitor or evaluator on the name expression
    void accept_visitor(expr_visitor& visitor) override;
    Object accept_evaluator(expr_evaluator& evaluator) override;
  };


  // Base class for an expression visitor
  class expr_visitor
  {
  public:
    virtual void visit_literal(expr_literal& expr) = 0;
    virtual void visit_name(expr_name& expr) = 0;
  };


  // Base class for an expression evaluator
  class expr_evaluator
  {
  public:
    virtual Object evaluate_literal(expr_literal& expr) = 0;
    virtual Object evaluate_name(expr_name& expr) = 0;
  };
}
