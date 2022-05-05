#include "interpreter.hpp"

namespace dauw
{

  void Interpreter::print_depth()
  {
    for (int i = 0; i < depth; i ++)
      fmt::print("  ");
  }

  void Interpreter::print(const std::shared_ptr<dauw::Expr>& expr)
  {
    expr->accept(shared_from_this());
  }

  // Visit a literal expression
  void Interpreter::visit_literal(const std::shared_ptr<dauw::ExprLiteral>& expr)
  {
    fmt::print("ExprLiteral({})\n", expr->value_);
  }

  // Visit a name expression
  void Interpreter::visit_name(const std::shared_ptr<dauw::ExprName>& expr)
  {
    fmt::print("ExprName({})\n", expr->name_.value());
  }

  // Visit a parenthesized expression
  void Interpreter::visit_parenthesized(const std::shared_ptr<dauw::ExprParenthesized>& expr)
  {
    fmt::print("ExprParenthesized:\n");

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->expr_);
    depth --;
  }

  // Visit a call expression
  void Interpreter::visit_call(const std::shared_ptr<dauw::ExprCall>& expr)
  {
    fmt::print("ExprCall:\n");

    depth ++;

    print_depth();
    fmt::print("callee:\n");
    depth ++;
    print_depth();
    print(expr->callee_);
    depth --;

    print_depth();
    fmt::print("arguments:\n");
    for (auto argument : expr->arguments_)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(argument);
      depth --;
    }

    depth --;
  }

  // Visit a subscript expression
  void Interpreter::visit_subscript(const std::shared_ptr<dauw::ExprSubscript>& expr)
  {
    fmt::print("ExprSubscript:\n");

    depth ++;

    print_depth();
    fmt::print("indexee:\n");
    depth ++;
    print_depth();
    print(expr->indexee_);
    depth --;

    print_depth();
    fmt::print("arguments:\n");
    for (auto argument : expr->arguments_)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(argument);
      depth --;
    }

    depth --;
  }

  // Visit a get expression
  void Interpreter::visit_get(const std::shared_ptr<dauw::ExprGet>& expr)
  {
    fmt::print("ExprGet:\n");

    depth ++;

    print_depth();
    fmt::print("object:\n");
    depth ++;
    print_depth();
    print(expr->object_);
    depth --;

    print_depth();
    fmt::print("name: '{}'\n", expr->name_.value());

    depth --;
  }

  // Visit an unary expression
  void Interpreter::visit_unary(const std::shared_ptr<dauw::ExprUnary>& expr)
  {
    fmt::print("ExprUnary('{}'):\n", expr->op_.value());

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->right_);
    depth --;
  }

  // Visit a binary expression
  void Interpreter::visit_binary(const std::shared_ptr<dauw::ExprBinary>& expr)
  {
    fmt::print("ExprBinary('{}'):\n", expr->op_.value());

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->left_);
    depth --;

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->right_);
    depth --;
  }

  // Visit a block expression
  void Interpreter::visit_block(const std::shared_ptr<dauw::ExprBlock>& expr)
  {
    fmt::print("ExprBlock:\n");

    for (auto i = 0; i < expr->exprs_.size(); i ++)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(expr->exprs_[i]);
      depth --;
    }
  }
}
