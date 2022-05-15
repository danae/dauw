#include "dauw.hpp"


namespace dauw
{
  void print_tokens(std::deque<Token> tokens)
  {
    int indent = 0;
    for (auto token : tokens)
    {
      auto indent_str = string_utils::repeat("    ", indent);

      if (token.name() == "newline")
      {
        fmt::print(fmt::fg(fmt::color::gold), "{}{}\n", indent_str, token);
      }
      else if (token.name() == "indent")
      {
        fmt::print(fmt::fg(fmt::color::light_green), "{}{}\n", indent_str, token);
        indent ++;
      }
      else if (token.name() == "dedent")
      {
        indent --;
        indent_str = string_utils::repeat("    ", indent);
        fmt::print(fmt::fg(fmt::color::light_pink), "{}{}\n", indent_str, token);
      }
      else if (token.name() == "eof")
      {
        fmt::print(fmt::fg(fmt::color::tomato), "{}{}\n", indent_str, token);
      }
      else if (token.name() == "comment")
      {
        fmt::print(fmt::fg(fmt::color::gray), "{}{}\n", indent_str, token);
      }
      else
      {
        fmt::print("{}{}\n", indent_str, token);
      }
    }
  }


  // Constructor
  Dauw::Dauw()
  {
    // Create the virtual machine
    vm_ = std::make_shared<VM>();
  }

  // Interpret a source string
  int Dauw::run(string_t source, string_t source_name)
  {
    // Set the current source and error indicator
    current_source_ = source;
    current_source_name_ = source_name;

    // Create the error reporter
    auto error_reporter = std::make_shared<ErrorReporter>(source, source_name);

    // Tokenize the source string
    auto tokens = Lexer(source, source_name).tokenize();

    // Parse the source string and exit the application if a parser error occurred
    auto expr = Parser(vm_.get(), error_reporter.get(), tokens).parse();
    if (error_reporter->had_error_)
      return DAUW_EXIT_DATAERR;

    // Execute the source string and exit the application if a runtime error occurred
    try
    {
      // Resolve the types of the expression
      //auto type_resolver = std::make_shared<TypeResolver>();
      //type_resolver->resolve(expr);

      // Evaluate the expression
      auto interpreter = std::make_shared<Interpreter>();
      interpreter->print(expr);

      // Quit with an ok exit code
      return DAUW_EXIT_OK;
    }
    catch (dauw::RuntimeError& error)
  	{
      error_reporter->report_error(error, "RuntimeError");
      return DAUW_EXIT_SOFTWAREERR;
  	}
  }

  // Run code from a read-eval-print loop
  int Dauw::run_repl()
  {
    // Variable to hold the current line
  	string_t line;

    // Execute the read-eval-print loop
    while (true)
  	{
      // Read a line from the prompt and quit if appropriate
      if (linenoise::Readline("$ ", line))
        break;

      // Add the line to the history
      linenoise::AddHistory(line.c_str());

      // Run the line
  		run(line, "<prompt>");
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
      fmt::print(fmt::fg(fmt::color::crimson), "The file '{}' does not exist", file);
      return DAUW_EXIT_IOERR;
    }

    // Read the source string from the absolute file
    std::ifstream stream(file);
  	string_t source((std::istreambuf_iterator<char>(stream)), (std::istreambuf_iterator<char>()));

    // Run the source string
  	return run(source, std::filesystem::canonical(file));
  }
}
