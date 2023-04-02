#pragma once

#include <dauw/common.hpp>
#include <dauw/code/location.hpp>
#include <dauw/code/source.hpp>

#include <utility>


namespace dauw
{
  // Class that defines an error
  class Error : public Exception
  {
    private:
      // The location where the error occurred
      Location location_;


    public:
      // Constructor
      inline Error(Location location, string_t message, Exception* previous) : Exception(message, previous), location_(location) {}
      inline Error(Location location, string_t message)  : Error(location, message, nullptr) {}

      // Return the location where the error occurred
      inline Location& location() { return location_; }

      // Return the type of the error as a string
      inline virtual string_t type() { return "Error"; }
  };


  // Error thrown when compiling a source file fails
  class CompilerError : public Error
  {
    public:
      inline CompilerError(Location location, string_t message, Exception* previous) : Error(location, message, previous) {}
      inline CompilerError(Location location, string_t message) : Error(location, message, nullptr) {}

      inline virtual string_t type() override { return "CompilerError"; }
  };

  // Error thrown when lexing or parsing a source file fails
  class SyntaxError : public CompilerError
  {
    public:
      inline SyntaxError(Location location, string_t message, Exception* previous) : CompilerError(location, message, previous) {}
      inline SyntaxError(Location location, string_t message) : CompilerError(location, message, nullptr) {}

      inline virtual string_t type() override { return "SyntaxError"; }
  };

  // Error thrown when resolving a type fails
  class TypeError : public CompilerError
  {
    public:
      // Constructor
      inline TypeError(Location location, string_t message, Exception* previous) : CompilerError(location, message, previous) {}
      inline TypeError(Location location, string_t message) : CompilerError(location, message, nullptr) {}

      inline virtual string_t type() override { return "TypeError"; }
  };

  // Error thrown when a type cannot be resolved
  class TypeUnresolvedError : public TypeError
  {
    public:
      inline TypeUnresolvedError(Location location, string_t message, Exception* previous) : TypeError(location, message, previous) {}
      inline TypeUnresolvedError(Location location, string_t message) : TypeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "TypeUnresolvedError"; }
  };

  // Error thrown when a type does not match an already declared type
  class TypeMismatchError : public TypeError
  {
    public:
      inline TypeMismatchError(Location location, string_t message, Exception* previous) : TypeError(location, message, previous) {}
      inline TypeMismatchError(Location location, string_t message) : TypeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "TypeMismatchError"; }
  };

  // Error thrown when processing a value fails
  class ValueError : public CompilerError
  {
    public:
      inline ValueError(Location location, string_t message, Exception* previous) : CompilerError(location, message, previous) {}
      inline ValueError(Location location, string_t message) : CompilerError(location, message, nullptr) {}

      inline virtual string_t type() override { return "ValueError"; }
  };

  // Error thrown when a value is not of the expected value type
  class ValueMismatchError : public ValueError
  {
    public:
      inline ValueMismatchError(Location location, string_t message, Exception* previous) : ValueError(location, message, previous) {}
      inline ValueMismatchError(Location location, string_t message) : ValueError(location, message, nullptr) {}

      inline virtual string_t type() override { return "ValueMismatchError"; }
  };

  // Error thrown when a value exceeds the range of the expected value type
  class ValueOverflowError : public ValueError
  {
    public:
      inline ValueOverflowError(Location location, string_t message, Exception* previous) : ValueError(location, message, previous) {}
      inline ValueOverflowError(Location location, string_t message) : ValueError(location, message, nullptr) {}

      inline virtual string_t type() override { return "ValueOverflowError"; }
  };

  // Error thrown when running a compiled a source file fails
  class RuntimeError : public Error
  {
    public:
      inline RuntimeError(Location location, string_t message, Exception* previous) : Error(location, message, previous) {}
      inline RuntimeError(Location location, string_t message) : Error(location, message, nullptr) {}

      inline virtual string_t type() override { return "RuntimeError"; }
  };

  // Error thrown when an aritmethic operation fails
  class ArithmeticError : public RuntimeError
  {
    public:
      inline ArithmeticError(Location location, string_t message, Exception* previous) : RuntimeError(location, message, previous) {}
      inline ArithmeticError(Location location, string_t message) : RuntimeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "ArithmeticError"; }
  };

  // Error thrown when a value is divided by zero
  class DivisionByZeroError : public ArithmeticError
  {
    public:
      inline DivisionByZeroError(Location location, string_t message, Exception* previous) : ArithmeticError(location, message, previous) {}
      inline DivisionByZeroError(Location location, string_t message) : ArithmeticError(location, message, nullptr) {}

      inline virtual string_t type() override { return "DivisionByZeroError"; }
  };

  // Error thrown when a value cannot be converted in an arithmetic operation
  class ConversionError : public ArithmeticError
  {
    public:
      inline ConversionError(Location location, string_t message, Exception* previous) : ArithmeticError(location, message, previous) {}
      inline ConversionError(Location location, string_t message) : ArithmeticError(location, message, nullptr) {}

      inline virtual string_t type() override { return "ConversionError"; }
  };

  // Error thrown when the stack of the virtual machine overflows, i.e. has reached its maximum capacity
  class StackOverflowError : public RuntimeError
  {
    public:
      inline StackOverflowError(Location location, string_t message, Exception* previous) : RuntimeError(location, message, previous) {}
      inline StackOverflowError(Location location, string_t message) : RuntimeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "StackOverflowError"; }
  };

  // Error thrown when the stack of the virtual machine underflows, i.e. has no more values
  class StackUnderflowError : public RuntimeError
  {
    public:
      inline StackUnderflowError(Location location, string_t message, Exception* previous) : RuntimeError(location, message, previous) {}
      inline StackUnderflowError(Location location, string_t message) : RuntimeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "StackUnderflowError"; }
  };

  // Error thrown when an operation is not implemented
  class UnimplementedError : public RuntimeError
  {
    public:
      inline UnimplementedError(Location location, string_t message, Exception* previous) : RuntimeError(location, message, previous) {}
      inline UnimplementedError(Location location, string_t message) : RuntimeError(location, message, nullptr) {}

      inline virtual string_t type() override { return "UnimplementedError"; }
  };


  // Class that defines an error reporter
  class Reporter
  {
    private:
      // The source file which errors are reported for
      source_ptr source_;

      // The list of reported errors
      std::vector<Error> errors_;


    public:
      // Constructor
      Reporter(source_ptr source);

      // Return the source file which errors are reported for
      source_ptr source();

      // Return the reported errors
      std::vector<Error> errors();

      // Return if the reporter has reported errors
      bool has_errors();

      // Clear the reported errors
      void clear_errors();

      // Print the reported errors
      void print_errors();

      // Print an error
      void print_error(Error error);

      // Report an error
      template <typename T, typename std::enable_if<std::is_base_of<Error, T>::value>::type* = nullptr>
      inline T report(Location location, string_t message, Exception* previous = nullptr)
      {
        T error(location, message, previous);
        errors_.push_back(error);
        return error;
      }
  };


  // Class that defines another class to be aware of an error reporter
  class ReporterAware
  {
    private:
      // Reference to the error reporter
      Reporter* reporter_;


    public:
      // Constructor
      inline ReporterAware(Reporter* reporter) : reporter_(reporter) {}

      // Report an error
      template <typename T, typename std::enable_if<std::is_base_of<Error, T>::value>::type* = nullptr>
      inline T report(Location location, string_t message, Exception* previous = nullptr)
      {
        return reporter_->report<T>(location, message, previous);
      }
  };
}
