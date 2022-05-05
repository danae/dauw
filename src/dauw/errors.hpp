#pragma once

#include <dauw/common.hpp>
#include <dauw/source/location.hpp>


namespace dauw
{
  // Class that defines a general error in the execution of the interpreter
  class Error
  {
    private:
      // The location where the error occurred
      Location location_;

      // The message of the error
      string_t message_;

      // The previous error that caused the error to happen
      Error* previous_;

    public:
      // Constructor
      Error(Location location, string_t message, Error* previous);
      Error(Location location, string_t message);

      // Return the fields of the error
      string_t message() { return message_; }
      Location location() { return location_; }
      Error* previous() { return previous_; }
  };


  // Class that defines a syntax error
  class SyntaxError : public Error
  {
    public:
      // Constructor
      SyntaxError(Location location, string_t message, Error* previous);
      SyntaxError(Location location, string_t message);
  };


  // Class that defines a runtime error
  class RuntimeError : public Error
  {
    public:
      // Constructor
      RuntimeError(Location location, string_t message, Error* previous);
      RuntimeError(Location location, string_t message);
  };
}
