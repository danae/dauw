#pragma once

#include <dauw/common.hpp>


namespace dauw
{
  // Enum that defines the instructions for the virtual machine
  enum class Instruction : uint8_t
  {
    // [] → []: Do nothing
    NOP,

    // Return
    RETURN,

    // [] → [Nothing]: Pushes a Nothing literal onto the stack
    NIL,

    // [] → [Bool]: Pushes a Bool literal with value false onto the stack
    BOOL_FALSE,

    // [] → [Bool]: Pushes a Bool literal with value true onto the stack
    BOOL_TRUE,

    // [] → [Int]: Pushes an Int constant onto the stack
    INT_CONST,

    // [] → [Real]: Pushes a Real constant onto the stack
    REAL_CONST,

    // [] → [Rune]: Pushes a Rune constant onto the stack
    RUNE_CONST,

    // [Nothing] → []: Pops nothing from the stack
    NIL_POP,

    // [Bool] → []: Pops a Bool value from the stack
    BOOL_POP,

    // [Nothing] → []: Pops an Int value from the stack
    INT_POP,

    // [Nothing] → []: Pops a Real value from the stack
    REAL_POP,

    // [Nothing] → []: Pops a Rune value from the stack
    RUNE_POP,

    // [Int] → [Bool]: Pops α: Int and pushes α == 0: Bool
    INT_ISZERO,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes -1: Int if α < β, 0: Int if α == β, and 1: Int if α > β
    INT_CMP,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α == β: Bool
    INT_EQ,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α <> β: Bool
    INT_NEQ,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α < β: Bool
    INT_LT,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α <= β: Bool
    INT_LTE,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α > β: Bool
    INT_GT,

    // [Int, Int] → [Bool]: Pops α: Int and β: Int and pushes α >= β: Bool
    INT_GTE,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes α + β: Int
    INT_ADD,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes α - β: Int
    INT_SUB,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes α * β: Int
    INT_MUL,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes α // β: Int, defined as the quotient of α divided by β using floor division
    INT_FDIV,

    // [Int, Int] → [Int]: Pops α: Int and β: Int and pushes α % β: Int, defined as the remainder of α divided by β using floor division
    INT_MOD,

    // [Int] → [Int]: Pops α: Int and pushes -α: Int
    INT_NEG,

    // [Int] → [Int]: Pops α: Int and pushes abs(α): Int
    INT_ABS,

    // [Int] → [Int]: Pops α: Int and pushes signum(α): Int
    INT_SIGN,

    // [Int] → [Real]: Pops α: Int and pushes α: Real renterpreted as a real value
    INT_CONV_REAL,

    // [Int] → [Rune]: Pops α: Int and pushes α: Rune renterpreted as an rune value including checking of bounds
    INT_CONV_RUNE,

    // [Real] → [Bool]: Pops α: Real and pushes true: Bool if α is NaN, false: Bool otherwise
    REAL_ISNAN,

    // [Real] → [Bool]: Pops α: Real and pushes α == 0.0: Bool
    REAL_ISZERO,

    // [Real, Real] → [Int]: Pops α: Real and β: Real and pushes -1: Int if α < β, 0: Int if α == β, and 1: Int if α > β
    REAL_CMP,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α == β: Bool
    REAL_EQ,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α <> β: Bool
    REAL_NEQ,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α < β: Bool
    REAL_LT,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α <= β: Bool
    REAL_LTE,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α > β: Bool
    REAL_GT,

    // [Real, Real] → [Bool]: Pops α: Real and β: Real and pushes α >= β: Bool
    REAL_GTE,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α + β: Real
    REAL_ADD,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α - β: Real
    REAL_SUB,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α * β: Real
    REAL_MUL,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α / β: Real
    REAL_DIV,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α // β: Real, defined as the quotient of α divided by β using floor division
    REAL_FDIV,

    // [Real, Real] → [Real]: Pops α: Real and β: Real and pushes α % β: Real, defined as the remainder of α divided by β using floor division
    REAL_MOD,

    // [Real] → [Real]: Pops α: Real and pushes -α: Real
    REAL_NEG,

    // [Real] → [Real]: Pops α: Real and pushes abs(α): Real
    REAL_ABS,

    // [Real] → [Real]: Pops α: Real and pushes signum(α): Real
    REAL_SIGN,

    // [Real] → [Int]: Pops α: Real and pushes trunc(α): Int, rounded towards zero
    REAL_CONV_TRUNC,

    // [Real] → [Int]: Pops α: Real and pushes floor(α): Int, rounded towards negative infinity
    REAL_CONV_FLOOR,

    // [Real] → [Int]: Pops α: Real and pushes ceail(α): Int, rounded towards positive infinity
    REAL_CONV_CEIL,

    // [Rune] → [Int]: Pops α: Rune and pushes α: Int renterpreted as an int value
    RUNE_CONV_INT,
  };
}
