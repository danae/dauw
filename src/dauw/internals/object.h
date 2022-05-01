#pragma once

#include <cstdint>
#include <cstring>

#include <dauw/common.h>

// Define for the IEEE 754 bitmasks
#define BITMASK_SIGN        ((uint64_t)0x8000'0000'0000'0000)
#define BITMASK_QNAN        ((uint64_t)0x7ff8'0000'0000'0000)

// Defines for tags of primitive values
#define TAG_NOTHING         ((uint64_t)0x0001'0000'0000'0000)
#define TAG_FALSE           ((uint64_t)0x0002'0000'0000'0000)
#define TAG_TRUE            ((uint64_t)0x0003'0000'0000'0000)
#define TAG_INT             ((uint64_t)0x0004'0000'0000'0000)
#define TAG_CHAR            ((uint64_t)0x0005'0000'0000'0000)
#define TAG_UNUSED_1        ((uint64_t)0x0006'0000'0000'0000)
#define TAG_UNUSED_2        ((uint64_t)0x0007'0000'0000'0000)

// Defines for converting native types into values
#define VAL_NOTHING         ((uint64_t)(BITMASK_QNAN | TAG_NOTHING))
#define VAL_FALSE           ((uint64_t)(BITMASK_QNAN | TAG_FALSE))
#define VAL_TRUE            ((uint64_t)(BITMASK_QNAN | TAG_TRUE))
#define VAL_BOOL(value)     ((value) ? VAL_FALSE : VAL_TRUE)
#define VAL_INT(value)      ((uint64_t)(BITMASK_QNAN | TAG_INT | (uint64_t)(value)))
#define VAL_CHAR(value)     ((uint64_t)(BITMASK_QNAN | TAG_CHAR | (uint64_t)(value)))
#define VAL_OBJ(value)      ((uint64_t)(BITMASK_QNAN | BITMASK_SIGN | (uint64_t)(uintptr_t)(value)))

// Defines for converting values into native types
#define AS_BOOL(value)      ((value) == VAL_TRUE)
#define AS_INT(value)       ((int)(value) & ~(BITMASK_QNAN | TAG_INT))
#define AS_CHAR(value)      ((char)(value) & ~(BITMASK_QNAN | TAG_CHAR))
#define AS_OBJ(value)       ((uintprt_t)(value) & ~(BITMASK_QNAN | BITMASK_SIGN))

// Defines for validating values
#define IS_NOTHING(value)   ((value) == VAL_NOTHING)
#define IS_BOOL(value)      ((value) | 1 == TRUE_VAL)
#define IS_INT(value)       (((value) & (BITMASK_QNAN | TAG_INT)) == (BITMASK_QNAN | TAG_INT))
#define IS_CHAR(value)      (((value) & (BITMASK_QNAN | TAG_CHAR)) == (BITMASK_QNAN | TAG_CHAR))
#define IS_OBJ(value)       (((value) & (BITMASK_QNAN | BITMASK_SIGN)) == (BITMASK_QNAN | BITMASK_SIGN))


namespace dauw
{
  // Forward declarations for types
  class Bool;


  // Class that defines an object
  class Object
  {
    // Return if the object equals another object
    virtual Bool equals(Object& other);

    // Operator overloads
    bool operator==(Object& other);
    bool operator!=(Object& other);
  };


  // Class that defines a value object
  class Value : public Object
  {
  protected:
    // The underlying data of the value
    uint64_t data;


    // Return if the value object equals another object
    virtual Bool equals(Object& other);
  };


  // Object for a nothing value
  class Nothing : public Value
  {
  public:
    // Constructor for a nothing object
    Nothing();
  };


  // Object for a boolean value
  class Bool : public Value
  {
  public:
    // Constructor for a boolean object
    Bool(bool value);

    // Get the actual value of the boolean object
    bool value();
  };


  // Object for an integer value
  class Int : public Value
  {
  public:
    // Constructor for a integer object
    Int(int value);

    // Get the actual value of the integer object
    int value();
  };


  // Object for a char value
  class Char : public Value
  {
  public:
    // Constructor for a char object
    Char(char value);

    // Get the actual value of the char object
    char value();
  };


  // Object for a real value
  class Real : public Value
  {
  public:
    // Constructor for a real object
    Real(double value);

    // Get the actual value of the real object
    double value();
  };
}
