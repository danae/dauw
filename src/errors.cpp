#include "errors.h"


namespace dauw
{
  // Return the string representation of the Error
  std::string Error::what()
  {
    return fmt::format("{} at {}", message(), location());
  }


  // Return the string representation of the source error
  std::string LexerError::what()
  {
    return fmt::format("{} while lexing the source at {}", message(), location());
  }
}
