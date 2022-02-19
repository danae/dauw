#include <fstream>
#include <iomanip>
#include <iostream>
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

		fmt::print(stdout, "Found {} tokens:\n", tokens.size());
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
void runFile(const char* path)
{
	// Read and run the contents of the file
	std::ifstream stream(path);
	std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	run(source);
}

// Main function
int main(int argc, const char* argv[])
{
	// Print the help info
	fmt::print(fmt::fg(fmt::color::plum), "dauw 0.1.0 -- Written by Danae Nova\n");

  // Check the number of arguments
	if (argc == 1)
	{
		// If there are no arguments, then run the REPL
		runPrompt();
		return 0;
	}
	else if (argc == 2)
	{
		// If there is one argument, then run the specified file
		runFile(argv[1]);
		return 0;
	}
	else
	{
		// Otherwise, invalid usage
	  fmt::print("Usage: dauw [file]\n");
	  return 64;
	}
}
