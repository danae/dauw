#include "interpreter.h"

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
    fmt::print(fmt::fg(fmt::color::light_green), "ExprLiteral('{}')\n", expr->value());
  }

  // Visit a name expression
  void Interpreter::visit_name(const std::shared_ptr<dauw::ExprName>& expr)
  {
    fmt::print(fmt::fg(fmt::color::light_green), "ExprName('{}')\n", expr->name().value());
  }

  // Visit a parenthesized expression
  void Interpreter::visit_parenthesized(const std::shared_ptr<dauw::ExprParenthesized>& expr)
  {
    fmt::print(fmt::fg(fmt::color::orange), "ExprParenthesized:\n");

    depth ++;

    print_depth();
    depth ++;
    print(expr->nested());
    depth --;

    depth --;
  }

  // Visit a call expression
  void Interpreter::visit_call(const std::shared_ptr<dauw::ExprCall>& expr)
  {
    fmt::print(fmt::fg(fmt::color::orange), "ExprCall:\n");

    depth ++;

    print_depth();
    fmt::print("callee:\n");
    depth ++;
    print_depth();
    print(expr->callee());
    depth --;

    print_depth();
    fmt::print("arguments:\n");
    for (auto argument : expr->arguments())
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
    fmt::print(fmt::fg(fmt::color::orange), "ExprSubscript:\n");

    depth ++;

    print_depth();
    fmt::print("callee:\n");
    depth ++;
    print_depth();
    print(expr->callee());
    depth --;

    print_depth();
    fmt::print("arguments:\n");
    for (auto argument : expr->arguments())
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
    fmt::print(fmt::fg(fmt::color::orange), "ExprGet:\n");

    depth ++;

    print_depth();
    fmt::print("callee:\n");
    depth ++;
    print_depth();
    print(expr->callee());
    depth --;

    print_depth();
    fmt::print("name: '{}'\n", expr->name().value());

    depth --;
  }

  // Visit an unary expression
  void Interpreter::visit_unary(const std::shared_ptr<dauw::ExprUnary>& expr)
  {
    fmt::print(fmt::fg(fmt::color::orange), "ExprUnary('{}'):\n", expr->op().value());

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->right());
    depth --;
  }

  // Visit a binary expression
  void Interpreter::visit_binary(const std::shared_ptr<dauw::ExprBinary>& expr)
  {
    fmt::print(fmt::fg(fmt::color::orange), "ExprBinary('{}'):\n", expr->op().value());

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->left());
    depth --;

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->right());
    depth --;
  }

  // Visit a block expression
  void Interpreter::visit_block(const std::shared_ptr<dauw::ExprBlock>& expr)
  {
    fmt::print(fmt::fg(fmt::color::orange), "ExprBlock:\n");

    for (auto i = 0; i < expr->exprs().size(); i ++)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(expr->exprs()[i]);
      depth --;
    }
  }
}
