#include "math.hpp"

namespace dauw::utils
{
  // Return the sign of an integer
  dauw_int_t sign(dauw_int_t n)
  {
    return (n > 0) ? 1 : ((n < 0) ? -1 : 0);
  }

  // Returns the sign of a real
  dauw_real_t sign(dauw_real_t n)
  {
    if (std::isnan(n))
      return n;
    else
      return (n > 0.0) ? 1.0 : ((n < 0.0) ? -1.0 : 0.0);
  }

  // Returns the floor division of two integers
  dauw_int_t floordiv(dauw_int_t numerator, dauw_int_t denominator)
  {
    if (denominator == 0)
      throw DivisionByZeroException("Division by zero");
    else
      return (int)(std::floor((double)numerator / (double)denominator));
  }

  // Returns the floor remainder of two integers
  dauw_int_t floormod(dauw_int_t numerator, dauw_int_t denominator)
  {
    auto quotient = floordiv(numerator, denominator);
    return numerator - quotient * denominator;
  }

  // Returns the floor division of two reals
  dauw_real_t floordiv(dauw_real_t numerator, dauw_real_t denominator)
  {
    if (std::isnan(numerator))
      return numerator;
    else if (std::isnan(denominator))
      return denominator;
    else
      return std::floor(numerator / denominator);
  }

  // Returns the floor remainder of two reals
  dauw_real_t floormod(dauw_real_t numerator, dauw_real_t denominator)
  {
    auto quotient = floordiv(numerator, denominator);
    return numerator - quotient * denominator;
  }
}
