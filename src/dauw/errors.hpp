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


  // Class that defines error reporter behaviour
  class ErrorReporter
  {
    public:
      // Type definition for the error list type
      using error_list_type = std::vector<Error>;


    private:
      // The list of reported errors
      error_list_type errors_;


    public:
      // Return the reported errors
      error_list_type errors();

      // Return if the reporter has reported errors
      bool has_errors();

      // Clear the reported errors
      void clear_errors();

      // Print the reported errors
      void print_errors(string_t source);

      // Print an error
      void print_error(Error error, string_t source);

      // Report an error at a location
      SyntaxError report_syntax_error(Location location, string_t message);
      RuntimeError report_runtime_error(Location location, string_t message);
  };
}
