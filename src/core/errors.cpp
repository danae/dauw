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

// Print the error to the console
void dauw::Error::print()
{
  fmt::print(fmt::fg(fmt::color::crimson), "{}\n", what());

  if (previous != NULL)
  {
    fmt::print(fmt::fg(fmt::color::crimson), "Caused by ");
    previous->print();
  }
}

//-----------------------------------------------------------------------------
