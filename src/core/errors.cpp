#include "errors.h"

//-----------------------------------------------------------------------------

// Constructor for an error
dauw::Error::Error(std::string message, dauw::Error* previous)
{
  this->message = message;
  this->previous = previous;
}
dauw::Error::Error(std::string message)
  : Error(message, NULL)
{
}

// Return the string representation of the error
std::string dauw::Error::what()
{
  return fmt::format("Error: {}", message);
}

// Report the error to the console
void dauw::Error::report()
{
  fmt::print("{}\n", what());

  auto error = previous;
  while (error != nullptr)
  {
    fmt::print("Caused by {}", error->what());
    error = error->previous;
  }
}

//-----------------------------------------------------------------------------
