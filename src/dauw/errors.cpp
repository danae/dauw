#include "errors.hpp"

namespace dauw
{
  // Constructor for an error reporter
  Reporter::Reporter(frontend::source_ptr source)
    : source_(source)
  {
  }

  // Return the source file which errors are reported for
  frontend::source_ptr Reporter::source()
  {
    return source_;
  }

  // Return the reported errors
  std::vector<Error> Reporter::errors()
  {
    return errors_;
  }

  // Return if the reporter has reported errors
  bool Reporter::has_errors()
  {
    return errors_.size() > 0;
  }

  // Clear the reported errors
  void Reporter::clear_errors()
  {
    errors_.clear();
  }

  // Print the reported errors
  void Reporter::print_errors()
  {
    for (auto error : errors_)
      print_error(error);
  }

  // Print an error
  void Reporter::print_error(Error error)
  {
    fmt::print(fmt::emphasis::bold | fmt::fg(fmt::color::crimson), "{}: ", error.type());
    fmt::print(fmt::fg(fmt::color::crimson), "{}\n", error.message());
    fmt::print("at {}\n", source_->format(error.location()));
  }
}
