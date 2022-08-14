#include "dauw.hpp"


namespace dauw
{
  // Constructor
  Dauw::Dauw()
  {
  }

  // Run code from a source file
  int Dauw::run(source_ptr source)
  {
    // TODO: Create a global reporter and VM

    // Create the error reporter
    auto reporter = std::make_shared<Reporter>(source);

    // Tokenize the source string
    auto tokens = Lexer(reporter.get(), source).tokenize();
    if (reporter->has_errors())
    {
      reporter->print_errors();
      reporter->clear_errors();
      return DAUW_EXIT_DATAERR;
    }

    // Parse the tokens and exit the application if a parser error occurred
    auto expr = Parser(reporter.get(), tokens).parse();
    if (reporter->has_errors())
    {
      reporter->print_errors();
      reporter->clear_errors();
      return DAUW_EXIT_DATAERR;
    }

    // Resolve the types of the expression
    auto type_resolver = std::make_shared<TypeResolver>(reporter.get());
    type_resolver->resolve(expr);
    if (reporter->has_errors())
    {
      reporter->print_errors();
      reporter->clear_errors();
      return DAUW_EXIT_SOFTWAREERR;
    }

    // Evaluate the expression and exit the application if a runtime error occurred
    auto interpreter = std::make_shared<Interpreter>(reporter.get());
    interpreter->evaluate(expr);
    if (reporter->has_errors())
    {
      reporter->print_errors();
      reporter->clear_errors();
      return DAUW_EXIT_SOFTWAREERR;
    }

    // Quit with an ok exit code
    return DAUW_EXIT_OK;
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
      auto source = Source::create("<prompt>", line);
  		run(source);
  	}

    // Exit the loop normally
    return DAUW_EXIT_OK;
  }

  // Run code from a file
  int Dauw::run_file(string_t file)
  {
    try
    {
      // Run the source from the specified file
      auto source = Source::create_read(file);
  	  return run(source);
    }
    catch (SourceException& ex)
    {
      fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex.message());
      return DAUW_EXIT_IOERR;
    }
  }
}
