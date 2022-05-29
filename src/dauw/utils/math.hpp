#pragma once

#include <dauw/common.hpp>


namespace dauw::utils
{
  // Return the sign of a number
  dauw_int_t sign(dauw_int_t n);
  dauw_real_t sign(dauw_real_t n);

  // Returns the floor division quotient and remainder of two numbers
  dauw_int_t floordiv(dauw_int_t numerator, dauw_int_t denominator);
  dauw_int_t floormod(dauw_int_t numerator, dauw_int_t denominator);
  dauw_real_t floordiv(dauw_real_t numerator, dauw_real_t denominator);
  dauw_real_t floormod(dauw_real_t numerator, dauw_real_t denominator);


  // Exception thrown when an aritmethic operation fails
  class ArithmeticException : public Exception
  {
    public:
      inline ArithmeticException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline ArithmeticException(string_t message) : Exception(message, nullptr) {}
  };

  // Exception thrown when a value is divided by zero
  class DivisionByZeroException : public ArithmeticException
  {
    public:
      inline DivisionByZeroException(string_t message, Exception* previous) : ArithmeticException(message, previous) {}
      inline DivisionByZeroException(string_t message) : ArithmeticException(message, nullptr) {}
  };

  // Exception thrown when a value cannot be converted in an arithmetic operation
  class ConversionException : public ArithmeticException
  {
    public:
      inline ConversionException(string_t message, Exception* previous) : ArithmeticException(message, previous) {}
      inline ConversionException(string_t message) : ArithmeticException(message, nullptr) {}
  };
}
