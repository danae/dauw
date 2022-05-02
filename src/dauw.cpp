#include "dauw.h"

namespace dauw
{
  // Constructor
  Dauw::Dauw()
  {
    // Create the interpreter
    interpreter_ = std::make_shared<Interpreter>();
  }

  // Interpret a source string
  int Dauw::run(string_t source, string_t source_name)
  {
    try
  	{
      // Set the current source and error indicator
      current_source_ = source;
      current_source_name_ = source_name;

      // Lex and parse the source string
      auto tokens = Lexer(source, source_name).tokenize();
      auto expr = Parser(tokens).parse();

      // Evaluate the expression
      if (expr != nullptr)
        interpreter_->print(expr);

      return DAUW_EXIT_OK;
  	}
  	catch (dauw::SyntaxError& error)
  	{
      report_error(error);
      return DAUW_EXIT_DATAERR;
  	}
    catch (dauw::RuntimeError& error)
  	{
      report_error(error);
      return DAUW_EXIT_SOFTWAREERR;
  	}
  }

  // Run code from a read-eval-print loop
  int Dauw::run_repl()
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
  		run(source, "<prompt>");
  	}

    // Exit the loop normally
    return DAUW_EXIT_OK;
  }

  // Run code from a file
  int Dauw::run_file(string_t file)
  {
    // Check if the file exists
    if (!std::filesystem::exists(file))
    {
      report(fmt::format("The file '{}' does not exist", file));
      return DAUW_EXIT_IOERR;
    }

    // Read the source string from the absolute file
    std::ifstream stream(file);
  	string_t source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));

    // Run the source string
  	return run(source, std::filesystem::canonical(file));
  }

  // Report an error
  void Dauw::report(string_t message)
  {
    // Print the message
    fmt::print(fmt::fg(fmt::color::crimson), message);
  }

  // Report an error with a location
  void Dauw::report(Location location, string_t message)
  {
    // Print the error
    report(message);

    // Get the location in the source
    auto lines = regex_lines(current_source_);
    if (location.line() >= lines.size())
      throw std::runtime_error(fmt::format("line {} is not present in the specified source", location.line() + 1));

    auto line = lines[location.line()];
    if (location.col() >= line.length())
      throw std::runtime_error(fmt::format("line {}, col {} is not present in the specified source", location.line() + 1, location.col() + 1));

    // Print the location
    fmt::print("at {}\n", location);
    fmt::print("{: >6d} | {}\n", location.line() + 1, lines[location.line()]);
    fmt::print("       | {}^\n", string_repeat(" ", location.col()));
  }

  // Report an error exception
  void Dauw::report_error(Error& error)
  {
    // Report the error
    report(error.location(), error.message());

    // Report the previous error if there is one
    if (error.previous() != nullptr)
  	{
  	  fmt::print("\nThe previous error was caused by:\n\n");
  		report_error(*error.previous());
  	}
  }
}
