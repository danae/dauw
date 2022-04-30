#pragma once

#include <string>

#include "common.h"
#include "source_location.h"


namespace dauw
{
  // Class that defines a general error in the execution of the interpreter
  class Error
  {
    protected:
      // The message of the error
      std::string m_message;

      // The location where the error occurred
      Location m_location;

      // The previous error that caused the error to happen
      Error* m_previous;

    public:
      // Constructor for an Error
      Error(std::string message, Location location, Error* previous);
      Error(std::string message, Location location);

      // Return the string representation of the error
      virtual std::string what();

      // Return the location of the error
      Location location();

      // Return the previous error that caused the error to happen
      Error* previous();
  };


  // Class that defines a lexer error
  class LexerError : public Error
  {
    public:
      // Constructor
      LexerError(std::string message, Location location, Error* previous);
      LexerError(std::string message, Location location);

      // Return the string representation of the error
      std::string what() override;
  };
}
