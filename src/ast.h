#pragma once

#include <functional>
#include <string>
#include <vector>

#include "common.h"
#include "token.h"


namespace dauw
{
  // Forward declarations
  class ExprVisitor;


  // Base class for an expression
  class Expr
  {
    public:
      // Return the location of the expression
      virtual Location location() = 0;

      // Accept a visitor on the expression
      virtual void accept(ExprVisitor& visitor) = 0;
  };


  // Class that defines a literal expression
  class LiteralExpr : public Expr
  {
    private:
      // The token of the expression
      Token& token_;

    public:
      // Constructor
      LiteralExpr(Token& token)
        : token_(token) { }

      // Return the value of the literal expression
      std::string value() { return token_.value(); }

      // Return the location of the expression
      Location location() override { return token_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a nested expression
  class NestedExpr : public Expr
  {
    private:
      // The name of the nested expression
      std::string name_;

      // The expressions of the nested expression
      Expr* left_;
      Expr* right_;

    public:
      // Constructor
      NestedExpr(std::string name, Expr* left, Expr* right)
        : name_(name), left_(left), right_(right) { }

      // Destructor
      ~NestedExpr();

      // Return the name of the nested expression
      std::string name() { return name_; }

      // Return the left and right expressions of the nested expression
      Expr* left() { return left_; }
      Expr* right() { return right_; }

      // Return the location of the expression
      Location location() override { return left_->location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Base class for an expression visitor
  class ExprVisitor
  {
    public:
      // Visit expressions
      virtual void visit_literal(LiteralExpr* expr) = 0;
      virtual void visit_nested(NestedExpr* expr) = 0;
  };
}
