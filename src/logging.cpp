#include "logging.h"


namespace dauw
{
  int depth = 0;

  void print_depth()
  {
    for (int i = 0; i < depth; i ++)
      fmt::print("  ");
  }


  void log_group(string_t message)
  {
    print_depth();
    fmt::print("▶ {}\n", message);

    depth ++;
  }
  void log_group_end()
  {
    depth --;
  }

  void log_verbose(string_t message)
  {
    print_depth();
    fmt::print(fmt::fg(fmt::color::gray), "{}\n", message);
  }
  void log_verbose(const fmt::text_style& text_style, string_t message)
  {
    print_depth();
    fmt::print(text_style, "{}\n", message);
  }

  // Report an error
  void log_error(string_t message, Location location, string_t source)
  {
    // Print the error information
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::crimson), "Error: {}\n", message);
    fmt::print(fmt::emphasis::bold, "at {}:\n", location);

    // Print the line where the error occurred
    fmt::print("{}\n", location.format(source));
  }

  void log_error(Error error, string_t source)
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
