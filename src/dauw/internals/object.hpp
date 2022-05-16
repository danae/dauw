#pragma once

#include <dauw/common.hpp>


namespace dauw
{
  // Enum that represents the type of an object
  enum class ObjType : char
  {
    STRING,
    LIST,
    RECORD,
    TYPE,
  };


  // Base class that represents an object in heap memory
  class Obj
  {
    private:
      // The type of the object
      ObjType type_;


    public:
      // Constructor
      Obj(ObjType type);

      // Destructor
      virtual ~Obj() = default;

      // Return the type of the object
      ObjType& type();

      // Return a representative string representation of the object
      string_t to_string();
  };
}
