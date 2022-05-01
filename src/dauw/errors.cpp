#include "errors.h"

namespace dauw
{
  // Return the string representation of the Error
  string_t Error::what()
  {
    return fmt::format("{} at {}", message(), location());
  }


  // Return the string representation of the source error
  string_t LexerError::what()
  {
    return fmt::format("{} while lexing the source at {}", message(), location());
  }
}
