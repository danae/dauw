#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>
#include <argh.h>
#include <fmt/color.h>
#include <fmt/core.h>

#include "common.h"
#include "errors.h"
#include "source_lexer.h"


// Report an error
void report(dauw::Error error, std::string source)
{
	// Print the error information
	fmt::print(fmt::fg(fmt::color::crimson), "{}\n", error.what());

	// Print the line where the error occurred
	fmt::print("{}\n", error.location().format(source));

  // Report the previous error if there is one
  if (error.previous() != nullptr)
	{
	  fmt::print(fmt::fg(fmt::color::crimson), "Caused by ");
		report(*error.previous(), source);
	}
}

// Run a string of source code
void run(std::string source)
{
	try
	{
		// Convert the source string to a deque of tokens
		std::deque<dauw::LexerToken> tokens = dauw::tokenize(source);

		fmt::print("Found {} tokens:\n", tokens.size());
	  for (auto token : tokens)
		  fmt::print(stdout, "- {}\n", token);
	}
	catch (dauw::Error& error)
	{
		report(error, source);
	}
}

// Run the read-eval-print loop
void run_prompt()
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
void run_file(std::string file)
{
	// Read and run the contents of the file
	std::ifstream stream(file);
	std::string source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));
	run(source);
}

// Print the header
void print_header(int argc, const char* argv[])
{
  fmt::print(fmt::emphasis::bold | fg(fmt::color::plum), "Dauw 0.1.0 -- written by Danae Nova\n");
  fmt::print("Interpreter for the Dauw programming language\n");
  fmt::print("Documentation and source available at https://dauw.dev/docs/\n\n");
}

// Print the usage
void print_usage(int argc, const char* argv[])
{
  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Usage\n\n");
	fmt::print("  $ {} [options] [file]\n\n", argv[0]);

  fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Positional arguments\n\n");
	fmt::print("  file            Evaluate the source code in the specified file. If no file\n");
	fmt::print("                  has been specified, then run the interactive prompt.\n\n");

	fmt::print(fmt::emphasis::bold | fmt::emphasis::underline, "Optional arguments\n\n");
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
