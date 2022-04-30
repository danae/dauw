#pragma once

#include "common.h"
#include "token.h"


namespace dauw
{
  // Class that defines a general error in the execution of the interpreter
  class Error
  {
    private:
      // The message of the error
      std::string message_;

      // The location where the error occurred
      Location location_;

      // The previous error that caused the error to happen
      Error* previous_;

    public:
      // Constructor for an Error
      Error(std::string message, Location location, Error* previous)
        : message_(message), location_(location), previous_(previous) { }
      Error(std::string message, Location location)
        : Error(message, location, nullptr) { }

      // Return the string representation of the error
      virtual std::string what();

      // Return the fields of the error
      std::string message() { return message_; }
      Location location() { return location_; }
      Error* previous() { return previous_; }
  };


  // Class that defines a lexer error
  class LexerError : public Error
  {
    public:
      // Constructor
      LexerError(std::string message, Location location, Error* previous)
        : Error(message, location, previous) { }
      LexerError(std::string message, Location location)
        : LexerError(message, location, nullptr) { }

      // Return the string representation of the error
      std::string what() override;
  };
}
