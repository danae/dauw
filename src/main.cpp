#include <argh.h>

#include "common.h"
#include "ast.h"
#include "errors.h"
#include "lexer.h"
#include "logging.h"
#include "parser.h"


class Printer : public dauw::ExprVisitor
{
  private:
	  int depth = 0;

	public:
		void print(dauw::Expr* expr)
		{
			expr->accept(*this);
		}

		void print_depth()
		{
			for (int i = 0; i < depth; i ++)
			  fmt::print("  ");
		}

		void visit_literal(dauw::LiteralExpr* expr)
		{
      fmt::print("{}\n", expr->value());
		}

		void visit_name(dauw::NameExpr* expr)
		{
      fmt::print(fmt::fg(fmt::color::slate_blue), "{}\n", expr->name());
		}

		void visit_parenthesized(dauw::ParenthesizedExpr* expr)
		{
			fmt::print(fmt::fg(fmt::color::orange), "parenthesized\n");

			print_depth();
			fmt::print("> ");

			depth ++;
			print(expr->nested());
			depth --;
		}

		void visit_call(dauw::CallExpr* expr)
		{
			fmt::print(fmt::fg(fmt::color::orange), "call\n");

			print_depth();
			fmt::print("> ");

			depth ++;
			print(expr->callee());
			depth --;

			for (auto argument : expr->arguments())
			{
				print_depth();
				fmt::print("> ");

				depth ++;
				print(argument);
				depth --;
			}
		}

		void visit_subscript(dauw::SubscriptExpr* expr)
		{
			fmt::print(fmt::fg(fmt::color::orange), "subscript\n");

			print_depth();
			fmt::print("> ");

			depth ++;
			print(expr->callee());
			depth --;

      for (auto argument : expr->arguments())
			{
			  print_depth();
			  fmt::print("> ");

			  depth ++;
			  print(argument);
			  depth --;
		  }
		}

		void visit_get(dauw::GetExpr* expr)
	  {
      fmt::print(fmt::fg(fmt::color::orange), "get '{}'\n", expr->name().value());

			print_depth();
			fmt::print("> ");

			depth ++;
			print(expr->callee());
			depth --;
		}

		void visit_unary(dauw::UnaryExpr* expr)
		{
			fmt::print(fmt::fg(fmt::color::orange), "unary {}\n", expr->op().value());

			print_depth();
			fmt::print("> ");

			depth ++;
			print(expr->right());
			depth --;
		}

		void visit_binary(dauw::BinaryExpr* expr)
		{
			fmt::print(fmt::fg(fmt::color::orange), "binary {}\n", expr->op().value());

			print_depth();
			fmt::print("left: ");

			depth ++;
			print(expr->left());
			depth --;

			print_depth();
			fmt::print("right: ");

			depth ++;
			print(expr->right());
			depth --;
		}
};

// Run a string of source code
void run(std::string name, std::string source)
{
	try
	{
		// Convert the source string to a deque of tokens
		dauw::Lexer lexer(name);
		std::deque<dauw::Token> tokens = lexer.tokenize(source);

    // Convert the deque of tokens to an expression
		dauw::Parser parser(tokens);
    dauw::Expr* expr = parser.parse();

    // Print the expression
		Printer printer;
		printer.print(expr);

		delete expr;
	}
	catch (dauw::Error& error)
	{
		dauw::log_error(error, source);
	}
}

// Run the read-eval-print loop
void run_prompt()
{
	// Variable to hold the current line
	std::string source;

	fmt::print("Use CTRL+C to exit the interactive prompt\n");

	// Execute the read-eval-print loop
	while (true)
	{
		// Read a new line and run it
		fmt::print(fmt::fg(fmt::color::slate_blue ), "$ ");

		std::getline(std::cin, source);
		run("<prompt>", source);
	}
}

// Run the file specified by the file path
void run_file(std::string path)
{
	// Read and run the contents of the file
	auto absolute_path = dauw::resolve_file(path);
	auto source = dauw::read_file(absolute_path);
	run(absolute_path, source);
}

// Print the header
void print_header(int argc, const char* argv[])
{
  fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::slate_blue), "Dauw 0.1.0 -- written by Danae Nova\n");
  fmt::print("Interpreter for the Dauw programming language\n");
  fmt::print("Documentation and source available at https://dauw.dev/docs/\n\n");
}

// Print the usage
void print_usage(int argc, const char* argv[])
{
  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Usage");
	fmt::print("\n\n");
	fmt::print("  $ {} [options] [file]\n\n", argv[0]);

  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Positional arguments");
	fmt::print("\n\n");
	fmt::print("  file            Evaluate the source code in the specified file. If no file\n");
	fmt::print("                  has been specified, then run the interactive prompt.\n\n");

	fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Optional arguments");
	fmt::print("\n\n");
	fmt::print("  -h, --help      Show this help message and exit.\n\n");
}

// Main function
int main(int argc, const char* argv[])
{
	// Parse the arguments
	argh::parser cmdl;
	cmdl.add_params({"-h", "--help"});
	cmdl.add_params({"-v", "--version"});
	cmdl.parse(argc, argv, argh::parser::SINGLE_DASH_IS_MULTIFLAG);

	// Handle the parsed arguments
	if (cmdl[{"-h", "--help"}])
	{
		// Show this help message and exit
		print_header(argc, argv);
		print_usage(argc, argv);
		return 0;
	}
	else if (cmdl[{"-v", "--version"}])
	{
		// Show version information and exit
		print_header(argc, argv);
		return 0;
	}
	else if (cmdl(1))
	{
		// Evaluate the source code in the specified file
		run_file(cmdl(1).str());
		return 0;
	}
	else
	{
		// If no file has been specified, then run the interactive prompt
		print_header(argc, argv);
		run_prompt();
	  return 0;
	}
}
