#include <fstream>
#include <iomanip>
#include <iostream>
#include <argh.h>
#include <fmt/color.h>
#include <fmt/core.h>

#include "debug.h"
#include "core/errors.h"
#include "parser/lexer.h"


// Run a string of source code
void run(std::string source)
{
	try
	{
		// Convert the source string to a deque of tokens
		dauw::Lexer lexer;
		std::deque<dauw::LexerToken> tokens = lexer.tokenize(source);

		fmt::print("Found {} tokens:\n", tokens.size());
	  for (auto token : tokens)
		  fmt::print(stdout, "- {}\n", formatLexerToken(token));
	}
	catch (dauw::Error& err)
	{
		err.print();
	}
}

// Run the read-eval-print loop
void runPrompt()
{
	// Variable to hold the current line
	std::string source;

	// Execute the read-eval-print loop
	while (true)
	{
		// Read a new line and run it
		fmt::print("dauw> ");
		std::getline(std::cin, source);
		run(source);
	}
}

// Run the file specified by the file path
void runFile(std::string file)
{
	fmt::print("{}", file);
	// Read and run the contents of the file
	std::ifstream stream(file);
	std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	run(source);
}

// Print the header
void printHeader(int argc, const char* argv[])
{
  fmt::print("Dauw 0.1.0 -- written by Danae Nova\n");
}

// Print the usage
void printUsage(int argc, const char* argv[])
{
  fmt::print("Usage: {} [options] [file]\n\n", argv[0]);

  fmt::print("Positional arguments:\n");
	fmt::print("  file            Evaluate the source code in the specified file.\n");
	fmt::print("                  If no file has been specified, then run the interactive prompt.\n\n");

	fmt::print("Optional arguments:\n");
	fmt::print("  -h, --help      Show this help message and exit.\n");
	fmt::print("  -v, --version   Show version information and exit.\n");
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
		printHeader(argc, argv);
		printUsage(argc, argv);
		return 0;
	}
	else if (cmdl[{"-v", "--version"}])
	{
		// Show version information and exit
		printHeader(argc, argv);
		return 0;
	}
	else if (cmdl(1))
	{
		// Evaluate the source code in the specified file
		runFile(cmdl(1).str());
		return 0;
	}
	else
	{
		// If no file has been specified, then run the interactive prompt
		printHeader(argc, argv);
		runPrompt();
	  return 0;
	}
}
