#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/internals/value.hpp>


namespace dauw::backend
{
  // Class that defines a code of bytecode
  class Code
  {
    public:
      // Type definition for the underlying byte type of the code
      using byte_type = uint8_t;


    private:
      // The bytes contained in the code
      std::vector<uint8_t> bytes_;

      // The source locations of the bytes in the code
      std::vector<frontend::Location> locations_;

      // The constants contained in the code
      std::vector<internals::Value> constants_;


    public:
      // Return the length of the bytes
      size_t bytes_length();

      // Return a byte at the specified index
      byte_type byte_at(size_t index);

      // Return a byte location at the specified index
      frontend::Location& byte_location_at(size_t index);

      // Append a byte to the code
      size_t append_byte(byte_type byte, frontend::Location location);

      // Return the length of the constants
      size_t constants_length();

      // Return a constant at the specified index
      internals::Value& constant_at(size_t index);

      // Append a constant to the code
      size_t append_constant(internals::Value value);
  };


  // Enum that defines the instructions for the virtual machine
  enum class Instruction : Code::byte_type
  {
    // No operation
    NOP,        // Performs no operation.

    // Constants
    NIL,        // Push nothing onto the stack.
    FALSE,      // Push false onto the stack.
    TRUE,       // Push true onto the stack.
    ICONST,     // Push the Int constant at index onto the stack.
    RCONST,     // Push the Real constant at index onto the stack.
    UCONST,     // Push the Rune constant at index onto the stack.

    // Arithmetic operations
    NOT,        // Negate the Bool α by calculating the logical complement, i.e. false becomes true and vice versa.
    INEG,       // Negate the Int α by calculating -α.
    RNEG,       // Negate the Real α by calculating -α.
    IABS,       // Calculate the absulute value of the Int α by calculating abs(α).
    RABS,       // Calculate the absulute value of the Real α by calculating abs(α).
    ISIGN,      // Calculate the sign of the Int α by calculating sign(α), which yields -1 if α is negative, 1 if α is positive, or 0 if α is zero.
    RSIGN,      // Calculate the sign of the Real α by calculating sign(α), which yields -1.0 if α is negative, 1.0 if α is positive, 0.0 if α is zero, or NaN if α is NaN.
    IADD,       // Add two Ints α and β by calculating α + β.
    RADD,       // Add two Reals α and β by calculating α + β.
    ISUB,       // Subtract two Ints α and β by calculating α - β.
    RSUB,       // Subtract two Reals α and β by calculating α - β.
    IMUL,       // Multiply two Ints α and β by calculating α * β.
    RMUL,       // Multiply two Reals α and β by calculating α * β.
    IDIV,       // Divide two Ints α and β by calculating α / β and returning the result as a Real.
    RDIV,       // Divide two Reals α and β by calculating α / β.
    IQUO,      // Calculate the quotient of dividing two Ints α and β using floor division.
    RQUO,      // Calculate the quotient of dividing two Reals α and β using floor division.
    IREM,       // Calculate the remainder of dividing two Ints α and β using floor division.
    RREM,       // Calculate the remainder of dividing two Reals α and β using floor division.

    // Relational operations
    BEQ,        // Compare two Bools α and β and yield true if α = β, false otherwise.
    ICMP,       // Compare two Ints α and β and yield -1 if α < β, 1 if α > β, or 0 if α = β.
    RCMP,       // Compare two Reals α and β and yield -1 if α < β, 1 if α > β, or 0 if α = β.
    UCMP,       // Compare two Runes α and β and yield -1 if α < β, 1 if α > β, or 0 if α = β.
    IEQZ,       // Yield true if the Int α equals zero by evaluating α = 0, false otherwise.
    INEZ,       // Yield true if the Int α does not equal zero by evaluating α ≠ 0, false otherwise.
    ILTZ,       // Yield true if the Int α is less than zero by evaluating α < 0, false otherwise.
    ILEZ,       // Yield true if the Int α is less than or equals zero by evaluating α ≤ 0, false otherwise.
    IGTZ,       // Yield true if the Int α is greater than zero by evaluating α > 0, false otherwise.
    IGEZ,       // Yield true if the Int α is greater than or equals zero by evaluating α ≥ 0, false otherwise.
    REQZ,       // Yield true if the Real α equals zero by evaluating α = 0.0, false otherwise.
    RNAN,       // Yield true if the Real α is a signalling NaN, false otherwise.

    // Conversions
    ITOR,       // Convert the Int α to a Real, preserving the exact value.
    ITOU,       // Convert Int α to a Rune, preserving the exact value if it fits in the bounds of a Rune, i.e. U+0000 ≤ α < U+D800 or U+DFFF < α ≤ U+10FFFF.
    RTRUNC,     // Convert the Real α to an Int by rounding the value to the nearest integer towards zero using trunc(α).
    RFLOOR,     // Convert the Real α to an Int by rounding the value to the nearest integer towards negative infinity using floor(α).
    RCEIL,      // Convert the Real α to an Int by rounding the value to the nearest integer towards positive infinity using ceil(α).
    UTOI,       // Convert the Rune α to an Int, preserving the exact value.

    // Consume operations
    POP,        // Pop the top from the stack.
    ECHO,       // Pop the top from the stack and print its value to the console.
  };
}
