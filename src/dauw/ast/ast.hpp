#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/value.hpp>
#include <dauw/source/location.hpp>
#include <dauw/source/token.hpp>


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

  class Interpreter;


  // Base class that defines an expression visitor
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


  // Base class thet defines an expression
  class Expr
  {
    public:
      // Destructor
      virtual ~Expr() = default;

      // Return the location of the expression
      virtual Location& location() = 0;

      // Accept a visitor on the expression
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) = 0;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a literal expression
  class ExprLiteral : public Expr, public std::enable_shared_from_this<ExprLiteral>
  {
    private:
      // The value of the expression
      Value value_;

      // The location of the expression
      Location location_;


    public:
      // Constructor
      ExprLiteral(Value value, Location location);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a name expression
  class ExprName : public Expr, public std::enable_shared_from_this<ExprName>
  {
    private:
      // The name token of the name expression
      Token name_;


    public:
      // Constructor
      ExprName(Token name);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a parenthesized expression
  class ExprParenthesized : public Expr, public std::enable_shared_from_this<ExprParenthesized>
  {
    private:
      // The nested expression of the parenthesized expression
      std::shared_ptr<Expr> expr_;


    public:
      // Constructor
      ExprParenthesized(std::shared_ptr<Expr> expr);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
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
      ExprCall(std::shared_ptr<Expr> callee, Token end, std::vector<std::shared_ptr<Expr>> arguments);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a subscript expression
  class ExprSubscript : public Expr, public std::enable_shared_from_this<ExprSubscript>
  {
    private:
      // The indexee of the subscript expression
      std::shared_ptr<Expr> indexee_;

      // The end token of the subscript expression
      Token end_;

      // The operand of the subscript expression
      std::vector<std::shared_ptr<Expr>> arguments_;


    public:
      // Constructor
      ExprSubscript(std::shared_ptr<Expr> indexee, Token end, std::vector<std::shared_ptr<Expr>> arguments);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a get expression
  class ExprGet : public Expr, public std::enable_shared_from_this<ExprGet>
  {
    private:
      // The object of the get expression
      std::shared_ptr<Expr> object_;

      // The name token of the get expression
      Token name_;


    public:
      // Constructor
      ExprGet(std::shared_ptr<Expr> object, Token name);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
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
      ExprUnary(Token op, std::shared_ptr<Expr> right);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
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
      ExprBinary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };


  // Class that defines a block expression
  class ExprBlock : public Expr, public std::enable_shared_from_this<ExprBlock>
  {
    private:
      // The expressions of the block expression
      std::vector<std::shared_ptr<Expr>> exprs_;


    public:
      // Constructor
      ExprBlock(std::vector<std::shared_ptr<Expr>> exprs);

      // Expression implementation
      virtual Location& location() override;
      virtual void accept(const std::shared_ptr<ExprVisitor>& visitor) override;

      // Friend classes
      friend class ExprVisitor;
      friend class Interpreter;
  };
}
