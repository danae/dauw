#include "object.h"

//-----------------------------------------------------------------------------

// Return if the object equals another object
dauw::Bool dauw::Object::equals(Object& other)
{
  return false;
}

// Operator overloads
bool dauw::Object::operator==(Object& other)
{
  return this->equals(other).value();
}
bool dauw::Object::operator!=(Object& other)
{
  return !(*this == other);
}

//-----------------------------------------------------------------------------

// Return if the value object equals another object
dauw::Bool dauw::Value::equals(Object& other)
{
  return dauw::Bool(false);
}

//-----------------------------------------------------------------------------

// Constructor for a nothing object
dauw::Nothing::Nothing()
{
  data = VAL_NOTHING;
}

//-----------------------------------------------------------------------------

// Constructor for a boolean object
dauw::Bool::Bool(bool value)
{
  data = VAL_BOOL(value);
}

// Get the actual value of the boolean object
bool dauw::Bool::value()
{
  return AS_BOOL(data);
}

//-----------------------------------------------------------------------------

// Constructor for a integer object
dauw::Int::Int(int value)
{
  data = VAL_INT(value);
}

// Get the actual value of the integer object
int dauw::Int::value()
{
  return AS_INT(data);
}

//-----------------------------------------------------------------------------

// Constructor for a char object
dauw::Char::Char(char value)
{
  data = VAL_CHAR(value);
}

// Get the actual value of the char object
char dauw::Char::value()
{
  return AS_CHAR(data);
}

//-----------------------------------------------------------------------------

// Constructor for a real object
dauw::Real::Real(double value)
{
  memcpy(&data, &value, sizeof(double));
}

// Get the actual value of the real object
double dauw::Real::value()
{
  double value;
  memcpy(&value, &data, sizeof(uint64_t));
  return value;
}

//-----------------------------------------------------------------------------
