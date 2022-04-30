#pragma once

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
      // The value of the expression
      std::string value_;

      // The location of the expression
      Location location_;

    public:
      // Constructor
      LiteralExpr(std::string value, Location location)
        : value_(value), location_(location) { }

      // Return the fields of the literal expression
      std::string value() { return value_; }

      // Return the location of the expression
      Location location() override { return location_; }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a name expression
  class NameExpr : public Expr
  {
    private:
      // The name token of the name expression
      Token name_;

    public:
      // Constructor
      NameExpr(Token name)
        : name_(name) { }

      // Return the fields of the name expression
      Token name() { return name_; }

      // Return the location of the expression
      Location location() override { return name_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a parenthesized expression
  class ParenthesizedExpr : public Expr
  {
    private:
      // The nested expression of the parenthesized expression
      Expr* nested_;

    public:
      // Constructor
      ParenthesizedExpr(Expr* nested)
        : nested_(nested) { }

      // Destructor
      ~ParenthesizedExpr();

      // Return the fields of the parenthesized expression
      Expr* nested() { return nested_; }

      // Return the location of the expression
      Location location() override { return nested_->location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a call expression
  class CallExpr : public Expr
  {
    private:
      // The callee of the call expression
      Expr* callee_;

      // The end token of the call expression
      Token end_;

      // The operand of the call expression
      std::vector<Expr*> arguments_;

    public:
      // Constructor
      CallExpr(Expr* callee, Token end, std::vector<Expr*> arguments)
        : callee_(callee), end_(end), arguments_(arguments) { }

      // Destructor
      ~CallExpr();

      // Return the fields of the call expression
      Expr* callee() { return callee_; }
      Token end() { return end_; }
      std::vector<Expr*> arguments() { return arguments_; }

      // Return the location of the expression
      Location location() override { return end_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a subscript expression
  class SubscriptExpr : public Expr
  {
    private:
      // The callee of the subscript expression
      Expr* callee_;

      // The end token of the subscript expression
      Token end_;

      // The operand of the subscript expression
      std::vector<Expr*> arguments_;

    public:
      // Constructor
      SubscriptExpr(Expr* callee, Token end, std::vector<Expr*> arguments)
        : callee_(callee), end_(end), arguments_(arguments) { }

      // Destructor
      ~SubscriptExpr();

      // Return the fields of the subscript expression
      Expr* callee() { return callee_; }
      Token end() { return end_; }
      std::vector<Expr*> arguments() { return arguments_; }

      // Return the location of the expression
      Location location() override { return end_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a get expression
  class GetExpr : public Expr
  {
    private:
      // The callee of the get expression
      Expr* callee_;

      // The name token of the get expression
      Token name_;

    public:
      // Constructor
      GetExpr(Expr* callee, Token name)
        : callee_(callee), name_(name) { }

      // Destructor
      ~GetExpr();

      // Return the fields of the get expression
      Expr* callee() { return callee_; }
      Token name() { return name_; }

      // Return the location of the expression
      Location location() override { return name_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines an unary expression
  class UnaryExpr : public Expr
  {
    private:
      // The operator of the unary expression
      Token op_;

      // The operand of the unary expression
      Expr* right_;

    public:
      // Constructor
      UnaryExpr(Token op, Expr* right)
        : op_(op), right_(right) { }

      // Destructor
      ~UnaryExpr();

      // Return the fields of the unary expression
      Token op() { return op_; }
      Expr* right() { return right_; }

      // Return the location of the expression
      Location location() override { return op_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Class that defines a binary expression
  class BinaryExpr : public Expr
  {
    private:
      // The operator of the binary expression
      Token op_;

      // The operands of the binary expression
      Expr* left_;
      Expr* right_;

    public:
      // Constructor
      BinaryExpr(Expr* left, Token op, Expr* right)
        : op_(op), left_(left), right_(right) { }

      // Destructor
      ~BinaryExpr();

      // Return the fields of the binary expression
      Token op() { return op_; }
      Expr* left() { return left_; }
      Expr* right() { return right_; }

      // Return the location of the expression
      Location location() override { return op_.location(); }

      // Accept a visitor on the expression
      void accept(ExprVisitor& visitor) override;
  };


  // Base class for an expression visitor
  class ExprVisitor
  {
    public:
      // Visit expressions
      virtual void visit_literal(LiteralExpr* expr) = 0;
      virtual void visit_name(NameExpr* expr) = 0;
      virtual void visit_parenthesized(ParenthesizedExpr* expr) = 0;
      virtual void visit_call(CallExpr* expr) = 0;
      virtual void visit_subscript(SubscriptExpr* expr) = 0;
      virtual void visit_get(GetExpr* expr) = 0;
      virtual void visit_unary(UnaryExpr* expr) = 0;
      virtual void visit_binary(BinaryExpr* expr) = 0;
  };
}
