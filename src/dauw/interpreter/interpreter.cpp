#include "interpreter.h"

namespace dauw
{
  // Interpret a source string
  int Interpreter::interpret(string_t source_name, string_t source)
  {
    try
  	{
      // Convert the source string to a deque of tokens
      Lexer lexer(source_name);
      auto tokens = lexer.tokenize(source);

      // Convert the deque of tokens to an expression
      Parser parser(tokens);
      auto expr = parser.parse();

      // Evaluate the expression
      if (expr != nullptr)
        print(expr);

      return 0;
  	}
  	catch (dauw::Error& error)
  	{
  		dauw::log_error(error, source);
      return 1;
  	}
  }

  // Interpret from a read-eval-print loop
  int Interpreter::interpret_from_repl()
  {
    // Variable to hold the current line
  	string_t source;

  	// Execute the read-eval-print loop
  	while (true)
  	{
  		// Print the prompts
  		fmt::print(fmt::fg(fmt::color::slate_blue ), "$ ");

      // Read a new line and run it
  		std::getline(std::cin, source);
  		interpret("<prompt>", source);
  	}

    return 0;
  }

  // Interpret from a file
  int Interpreter::interpret_from_file(string_t file_path)
  {
    // Read and run the contents of the file
  	auto absolute_path = resolve_file(file_path);
  	auto source = read_file(absolute_path);
  	return interpret(absolute_path, source);
  }

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
