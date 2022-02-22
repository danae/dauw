#pragma once

#include <string>
#include <fmt/core.h>
#include <fmt/color.h>

namespace dauw
{
  // Class that defines a general error in the execution of the interpreter
  class Error
  {
  public:
    // The message of the error
    std::string message;

    // The previous error that caused the error to happen
    Error* previous;


    // Constructor for an error
    Error(std::string message, Error* previous);
    Error(std::string message);

    // Return the string representation of the error
    virtual std::string what();

    // Report the error to the console
    void report();
  };
}
