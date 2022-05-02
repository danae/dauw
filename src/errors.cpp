#include "errors.h"

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
}
