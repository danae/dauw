#pragma once

#include "common.h"
#include "internals_value.h"
#include "source_location.h"
#include "source_token.h"


namespace dauw
{
  // Forward declarations
  class Expr;
  class ExprLiteral;
  class ExprName;
  class ExprParenthesized;
  class ExprCall;
  class ExprSubscript;
  class ExprGet;
  class ExprUnary;
  class ExprBinary;
  class ExprBlock;


  // Base class for an expression visitor
  class ExprVisitor
  {
    public:
      // Destructor
      virtual ~ExprVisitor() = default;

      // Visit expressions
      virtual void visit_literal(const std::shared_ptr<ExprLiteral>& expr) = 0;
      virtual void visit_name(const std::shared_ptr<ExprName>& expr) = 0;
      virtual void visit_parenthesized(const std::shared_ptr<ExprParenthesized>& expr) = 0;
      virtual void visit_call(const std::shared_ptr<ExprCall>& expr) = 0;
      virtual void visit_subscript(const std::shared_ptr<ExprSubscript>& expr) = 0;
      virtual void visit_get(const std::shared_ptr<ExprGet>& expr) = 0;
      virtual void visit_unary(const std::shared_ptr<ExprUnary>& expr) = 0;
      virtual void visit_binary(const std::shared_ptr<ExprBinary>& expr) = 0;
      virtual void visit_block(const std::shared_ptr<ExprBlock>& expr) = 0;
  };


  // Base class for an expression
  class Expr
  {
    public:
      // Destructor
      virtual ~Expr() = default;

      // Return the location of the expression
      virtual Location& location() = 0;

      // Accept a visitor on the expression
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) = 0;
  };


  // Class that defines a literal expression
  class ExprLiteral : public Expr, public std::enable_shared_from_this<ExprLiteral>
  {
    private:
      // The value of the expression
      value_t value_;

      // The location of the expression
      Location location_;


    public:
      // Constructor
      ExprLiteral(value_t value, Location location)
        : value_(value), location_(location) { }

      // Return the fields of the literal expression
      value_t& value() { return value_; }

      // Return the location of the expression
      Location& location() override { return location_; }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a name expression
  class ExprName : public Expr, public std::enable_shared_from_this<ExprName>
  {
    private:
      // The name token of the name expression
      Token name_;


    public:
      // Constructor
      ExprName(Token name)
        : name_(name) { }

      // Return the fields of the name expression
      Token& name() { return name_; }

      // Return the location of the expression
      Location& location() override { return name_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a parenthesized expression
  class ExprParenthesized : public Expr, public std::enable_shared_from_this<ExprParenthesized>
  {
    private:
      // The nested expression of the parenthesized expression
      std::shared_ptr<Expr> nested_;


    public:
      // Constructor
      ExprParenthesized(std::shared_ptr<Expr> nested)
        : nested_(nested) { }

      // Return the fields of the parenthesized expression
      std::shared_ptr<Expr>& nested() { return nested_; }

      // Return the location of the expression
      Location& location() override { return nested_->location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a call expression
  class ExprCall : public Expr, public std::enable_shared_from_this<ExprCall>
  {
    private:
      // The callee of the call expression
      std::shared_ptr<Expr> callee_;

      // The end token of the call expression
      Token end_;

      // The operand of the call expression
      std::vector<std::shared_ptr<Expr>> arguments_;


    public:
      // Constructor
      ExprCall(std::shared_ptr<Expr> callee, Token end, std::vector<std::shared_ptr<Expr>> arguments)
        : callee_(callee), end_(end), arguments_(arguments) { }

      // Return the fields of the call expression
      std::shared_ptr<Expr>& callee() { return callee_; }
      Token& end() { return end_; }
      std::vector<std::shared_ptr<Expr>>& arguments() { return arguments_; }

      // Return the location of the expression
      Location& location() override { return end_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a subscript expression
  class ExprSubscript : public Expr, public std::enable_shared_from_this<ExprSubscript>
  {
    private:
      // The callee of the subscript expression
      std::shared_ptr<Expr> callee_;

      // The end token of the subscript expression
      Token end_;

      // The operand of the subscript expression
      std::vector<std::shared_ptr<Expr>> arguments_;


    public:
      // Constructor
      ExprSubscript(std::shared_ptr<Expr> callee, Token end, std::vector<std::shared_ptr<Expr>> arguments)
        : callee_(callee), end_(end), arguments_(arguments) { }

      // Return the fields of the subscript expression
      std::shared_ptr<Expr>& callee() { return callee_; }
      Token& end() { return end_; }
      std::vector<std::shared_ptr<Expr>>& arguments() { return arguments_; }

      // Return the location of the expression
      Location& location() override { return end_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a get expression
  class ExprGet : public Expr, public std::enable_shared_from_this<ExprGet>
  {
    private:
      // The callee of the get expression
      std::shared_ptr<Expr> callee_;

      // The name token of the get expression
      Token name_;


    public:
      // Constructor
      ExprGet(std::shared_ptr<Expr> callee, Token name)
        : callee_(callee), name_(name) { }

      // Return the fields of the get expression
      std::shared_ptr<Expr>& callee() { return callee_; }
      Token& name() { return name_; }

      // Return the location of the expression
      Location& location() override { return name_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines an unary expression
  class ExprUnary : public Expr, public std::enable_shared_from_this<ExprUnary>
  {
    private:
      // The operator of the unary expression
      Token op_;

      // The operand of the unary expression
      std::shared_ptr<Expr> right_;


    public:
      // Constructor
      ExprUnary(Token op, std::shared_ptr<Expr> right)
        : op_(op), right_(right) { }

      // Return the fields of the unary expression
      Token& op() { return op_; }
      std::shared_ptr<Expr>& right() { return right_; }

      // Return the location of the expression
      Location& location() override { return op_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a binary expression
  class ExprBinary : public Expr, public std::enable_shared_from_this<ExprBinary>
  {
    private:
      // The operator of the binary expression
      Token op_;

      // The operands of the binary expression
      std::shared_ptr<Expr> left_;
      std::shared_ptr<Expr> right_;


    public:
      // Constructor
      ExprBinary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
        : op_(op), left_(left), right_(right) { }

      // Return the fields of the binary expression
      Token& op() { return op_; }
      std::shared_ptr<Expr>& left() { return left_; }
      std::shared_ptr<Expr>& right() { return right_; }

      // Return the location of the expression
      Location& location() override { return op_.location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };


  // Class that defines a block expression
  class ExprBlock : public Expr, public std::enable_shared_from_this<ExprBlock>
  {
    private:
      // The expressions of the block expression
      std::vector<std::shared_ptr<Expr>> exprs_;


    public:
      // ExprBlock
      ExprBlock(std::vector<std::shared_ptr<Expr>> exprs)
        : exprs_(exprs) { }

      // Return the fields of the block expression
      std::vector<std::shared_ptr<Expr>> exprs() { return exprs_; }

      // Return the location of the expression
      Location& location() override { return exprs_[0]->location(); }

      // Accept a visitor on the expression
      void accept(const std::shared_ptr<ExprVisitor>& visitor) override;
  };
}
