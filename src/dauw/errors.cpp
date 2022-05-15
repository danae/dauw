#include "errors.hpp"

namespace dauw
{
  // Constructor for an error
  Error::Error(Location location, string_t message, Error* previous)
    : location_(location), message_(message), previous_(previous)
  {
  }
  Error::Error(Location location, string_t message)
    : Error(location, message, nullptr)
  {
  }

  // --------------------------------------------------------------------------

  // Constructor for a syntax error
  SyntaxError::SyntaxError(Location location, string_t message, Error* previous)
    : Error(location, message, previous)
  {
  }
  SyntaxError::SyntaxError(Location location, string_t message)
    : SyntaxError(location, message, nullptr)
  {
  }

  // --------------------------------------------------------------------------

  // Constructor for a runtime error
  RuntimeError::RuntimeError(Location location, string_t message, Error* previous)
    : Error(location, message, previous)
  {
  }
  RuntimeError::RuntimeError(Location location, string_t message)
    : RuntimeError(location, message, nullptr)
  {
  }

  // --------------------------------------------------------------------------

  // Return the reported errors
  ErrorReporter::error_list_type ErrorReporter::errors()
  {
    return errors_;
  }

  // Return if the reporter has reported errors
  bool ErrorReporter::has_errors()
  {
    return errors_.size() > 0;
  }

  // Clear the reported errors
  void ErrorReporter::clear_errors()
  {
    errors_.clear();
  }


  // Print the reported errors
  void ErrorReporter::print_errors(string_t source)
  {
    // Iterate over the errors
    for (auto error : errors_)
    {
      print_error(error, source);
      fmt::print("\n");
    }
  }

  // Print an error
  void ErrorReporter::print_error(Error error, string_t source)
  {
    // Print the message
    fmt::print(fmt::fg(fmt::color::crimson), "{}\n", error.message());

    // Print the location
    auto lines = regex_utils::split_lines(source);
    if (error.location().line() >= lines.size())
      throw std::runtime_error(fmt::format("line {} is not present in the specified source", error.location().line() + 1));

    auto line = lines[error.location().line()];
    if (error.location().col() > line.length())
      throw std::runtime_error(fmt::format("line {}, col {} is not present in the specified source", error.location().line() + 1, error.location().col() + 1));

    fmt::print("at {}\n", error.location());
    fmt::print("{: >6d} | {}\n", error.location().line() + 1, lines[error.location().line()]);
    fmt::print("       | {}^\n", string_utils::repeat(" ", error.location().col()));

    // Print the previous error if any
    if (error.previous() != nullptr)
  	{
  	  fmt::print("\nThe previous error was caused by:\n\n");
  		print_error(*error.previous(), source);
  	}
  }

  // Report a syntax error at a location
  SyntaxError ErrorReporter::report_syntax_error(Location location, string_t message)
  {
    auto error = SyntaxError(location, message);
    errors_.push_back(error);
    return error;
  }

  // Report a runtime error at a location
  RuntimeError ErrorReporter::report_runtime_error(Location location, string_t message)
  {
    auto error = RuntimeError(location, message);
    errors_.push_back(error);
    return error;
  }
}
