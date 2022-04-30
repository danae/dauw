#include "logging.h"


namespace dauw
{
  // Report an error
  void log_error(std::string message, Location location, std::string source)
  {
    // Print the error information
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::crimson), "Error: {}\n", message);
    fmt::print(fmt::emphasis::bold, "at {}:\n", location);

    // Print the line where the error occurred
    fmt::print("{}\n", location.format(source));
  }

  void log_error(Error error, std::string source)
  {
    // Report the error
    log_error(error.message(), error.location(), source);

    // Report the previous error if there is one
    if (error.previous() != nullptr)
  	{
  	  fmt::print("\nThe previous error was caused by:\n\n");
  		log_error(*error.previous(), source);
  	}
  }
}
