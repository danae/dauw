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

  // Constructor for a syntax error
  SyntaxError::SyntaxError(Location location, string_t message, Error* previous)
    : Error(location, message, previous)
  {
  }
  SyntaxError::SyntaxError(Location location, string_t message)
    : SyntaxError(location, message, nullptr)
  {
  }

  // Constructor for a runtime error
  RuntimeError::RuntimeError(Location location, string_t message, Error* previous)
    : Error(location, message, previous)
  {
  }
  RuntimeError::RuntimeError(Location location, string_t message)
    : RuntimeError(location, message, nullptr)
  {
  }


  // Constructor for an error reporter
  ErrorReporter::ErrorReporter(string_t source, string_t source_name)
    : source_(source), source_name_(source_name)
  {
  }

  // Report an error
  void ErrorReporter::report(string_t message)
  {
    // Print the message
    fmt::print(fmt::fg(fmt::color::crimson), "{}\n", message);

    // Set the error state
    had_error_ = true;
  }

  // Report an error with a location
  void ErrorReporter::report(Location location, string_t message)
  {
    // Print the error
    report(message);

    // Get the location in the source
    auto lines = regex_utils::split_lines(source_);
    if (location.line() >= lines.size())
      throw std::runtime_error(fmt::format("line {} is not present in the specified source", location.line() + 1));

    auto line = lines[location.line()];
    if (location.col() > line.length())
      throw std::runtime_error(fmt::format("line {}, col {} is not present in the specified source", location.line() + 1, location.col() + 1));

    // Print the location
    fmt::print("at {}\n", location);
    fmt::print("{: >6d} | {}\n", location.line() + 1, lines[location.line()]);
    fmt::print("       | {}^\n", string_utils::repeat(" ", location.col()));
  }

  // Report an error exception
  void ErrorReporter::report_error(Error& error, string_t error_type)
  {
    // Report the error
    report(error.location(), fmt::format("{}: {}", error_type, error.message()));

    // Report the previous error if there is one
    if (error.previous() != nullptr)
  	{
  	  fmt::print("\nThe previous error was caused by:\n\n");
  		report_error(*error.previous());
  	}
  }
}
