#include "math.hpp"

namespace dauw::utils
{
  // Return the sign of an integer
  dauw_int_t sign(dauw_int_t n)
  {
    return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
  }

  // Return the sign of a float
  dauw_float_t sign(dauw_float_t n)
  {
    if (std::isnan(n))
      return n;
    else
      return (n > 0.0) ? 1.0 : ((n < 0.0) ? -1.0 : 0.0);
  }

  // Return the floor division of two integers
  dauw_int_t floordiv(dauw_int_t numerator, dauw_int_t denominator)
  {
    if (denominator == 0)
      throw DivisionByZeroException("Division by zero");
    else
      return (int)(std::floor((double)numerator / (double)denominator));
  }

  // Return the floor remainder of two integers
  dauw_int_t floormod(dauw_int_t numerator, dauw_int_t denominator)
  {
    auto quotient = floordiv(numerator, denominator);
    return numerator - quotient * denominator;
  }

  // Return the floor division of two floats
  dauw_float_t floordiv(dauw_float_t numerator, dauw_float_t denominator)
  {
    if (std::isnan(numerator))
      return numerator;
    else if (std::isnan(denominator))
      return denominator;
    else
      return std::floor(numerator / denominator);
  }

  // Return the floor remainder of two floats
  dauw_float_t floormod(dauw_float_t numerator, dauw_float_t denominator)
  {
    auto quotient = floordiv(numerator, denominator);
    return numerator - quotient * denominator;
  }
}
