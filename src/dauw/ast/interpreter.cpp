#include "interpreter.hpp"

namespace dauw
{

  void Interpreter::print_depth()
  {
    for (int i = 0; i < depth; i ++)
      fmt::print("  ");
  }

  void Interpreter::print(const expr_ptr& expr)
  {
    expr->accept(shared_from_this());
  }

  // Visit a literal expression
  void Interpreter::visit_literal(const expr_literal_ptr& expr)
  {
    fmt::print("ExprLiteral({})\n", expr->value(), expr->value().type());
  }

  // Visit a sequence expression
  void Interpreter::visit_sequence(const expr_sequence_ptr& expr)
  {
    fmt::print("ExprSequence\n");

    for (auto item : *expr)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(item);
      depth --;
    }
  }

  // Visit a record expression
  void Interpreter::visit_record(const expr_record_ptr& expr)
  {
    fmt::print("ExprRecord\n");

    depth ++;

    for (auto item : *expr)
    {
      print_depth();
      fmt::print("{}:\n", std::get<0>(item));
      depth ++;
      print_depth();
      print(std::get<1>(item));
      depth --;
    }

    depth --;
  }

  // Visit a name expression
  void Interpreter::visit_name(const expr_name_ptr& expr)
  {
    fmt::print("ExprName({})\n", expr->name());
  }

  // Visit a function expression
  void Interpreter::visit_function(const expr_function_ptr& expr)
  {
    fmt::print("ExprFunction\n");

    depth ++;

    print_depth();
    fmt::print("parameters:\n");
    for (auto parameter : expr->parameters())
    {
      print_depth();
      fmt::print("- {}\n", parameter);
    }

    if (expr->has_return_type())
    {
      print_depth();
      fmt::print("return type:\n");
      depth ++;
      print_depth();
      print(expr->return_type());
      depth --;
    }

    print_depth();
    fmt::print("body:\n");
    depth ++;
    print_depth();
    print(expr->body());
    depth --;

    depth --;
  }

  // Visit a grouped expression
  void Interpreter::visit_grouped(const expr_grouped_ptr& expr)
  {
    fmt::print("ExprGrouped\n");

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->expr());
    depth --;
  }

  // Visit a call expression
  void Interpreter::visit_call(const expr_call_ptr& expr)
  {
    fmt::print("ExprCall\n");

    depth ++;

    print_depth();
    fmt::print("callee:\n");
    depth ++;
    print_depth();
    print(expr->callee());
    depth --;

    print_depth();
    fmt::print("arguments:\n");
    for (auto argument : *expr->arguments())
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
  void Interpreter::visit_get(const expr_get_ptr& expr)
  {
    fmt::print("ExprGet\n");

    depth ++;

    print_depth();
    fmt::print("object:\n");
    depth ++;
    print_depth();
    print(expr->object());
    depth --;

    print_depth();
    fmt::print("name: '{}'\n", expr->name());

    depth --;
  }

  // Visit an unary expression
  void Interpreter::visit_unary(const expr_unary_ptr& expr)
  {
    fmt::print("ExprUnary('{}')\n", expr->op());

    print_depth();
    fmt::print("- ");
    depth ++;
    print(expr->right());
    depth --;
  }

  // Visit a binary expression
  void Interpreter::visit_binary(const expr_binary_ptr& expr)
  {
    fmt::print("ExprBinary('{}')\n", expr->op());

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

  // Visit an if expression
  void Interpreter::visit_if(const expr_if_ptr& expr)
  {
    fmt::print("ExprIf\n");

    print_depth();
    fmt::print("condition: ");
    depth ++;
    print(expr->condition());
    depth --;

    print_depth();
    fmt::print("true_branch: ");
    depth ++;
    print(expr->true_branch());
    depth --;

    if (expr->has_false_branch())
    {
      print_depth();
      fmt::print("false_branch ");
      depth ++;
      print(expr->false_branch());
      depth --;
    }
  }

  // Visit a for expression
  void Interpreter::visit_for(const expr_for_ptr& expr)
  {
    fmt::print("ExprFor\n");

    print_depth();
    fmt::print("name: {}\n", expr->name());

    print_depth();
    fmt::print("iterable: ");
    depth ++;
    print(expr->iterable());
    depth --;

    print_depth();
    fmt::print("body: ");
    depth ++;
    print(expr->body());
    depth --;
  }

  // Visit a while expression
  void Interpreter::visit_while(const expr_while_ptr& expr)
  {
    fmt::print("ExprWhile\n");

    print_depth();
    fmt::print("condition: ");
    depth ++;
    print(expr->condition());
    depth --;

    print_depth();
    fmt::print("body: ");
    depth ++;
    print(expr->body());
    depth --;
  }

  // Visit an until expression
  void Interpreter::visit_until(const expr_until_ptr& expr)
  {
    fmt::print("ExprUntil\n");

    print_depth();
    fmt::print("condition: ");
    depth ++;
    print(expr->condition());
    depth --;

    print_depth();
    fmt::print("body: ");
    depth ++;
    print(expr->body());
    depth --;
  }

  // Visit a block expression
  void Interpreter::visit_block(const expr_block_ptr& expr)
  {
    fmt::print("ExprBlock\n");

    for (auto sub_expr : *expr)
    {
      print_depth();
      fmt::print("- ");
      depth ++;
      print(sub_expr);
      depth --;
    }
  }

  // Visit a def expression
  void Interpreter::visit_def(const expr_def_ptr& expr)
  {
    fmt::print("ExprDef\n");

    depth ++;

    print_depth();
    fmt::print("name: {}\n", expr->name());

    print_depth();
    fmt::print("value:\n");
    depth ++;
    print_depth();
    print(expr->value());
    depth --;

    depth --;
  }
}
