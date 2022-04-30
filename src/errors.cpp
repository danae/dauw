#include "errors.h"


namespace dauw
{
  // Constructor for an error
  Error::Error(std::string message, Location location, Error* previous)
  {
    m_message = message;
    m_location = location;
    m_previous = m_previous;
  }
  Error::Error(std::string message, Location location)
  {
    m_message = message;
    m_location = location;
    m_previous = nullptr;
  }

  // Return the string representation of the Error
  std::string Error::what()
  {
    return fmt::format("Error: {} at {}", m_message, m_location);
  }

  // Return the location of the error
  Location Error::location()
  {
    return m_location;
  }

  // Return the previous error that caused the error to happen
  Error* Error::previous()
  {
    return m_previous;
  }


  // Constructor for a lexer error
  LexerError::LexerError(std::string message, Location location, Error* previous)
    : Error(message, location, previous)
  {
  }
  LexerError::LexerError(std::string message, Location location)
    : Error(message, location)
  {
  }

  // Return the string representation of the source error
  std::string LexerError::what()
  {
    return fmt::format("LexerError: {} at {}", m_message, m_location);
  }
}
