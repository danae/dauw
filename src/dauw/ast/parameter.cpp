#include "parameter.hpp"

namespace dauw
{
  // Constructor
  Parameter::Parameter(Token name, Type type)
    : name_(name), type_(type)
  {
  }

  // Return the name of the parameter
  string_t Parameter::name()
  {
    return name_.value();
  }

  // Return the type of the parameter
  Type Parameter::type()
  {
    return type_;
  }

  // Return the location of the parameter
  Location& Parameter::location()
  {
    return name_.location();
  }
}
