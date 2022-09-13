#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/type.hpp>


namespace dauw
{


  // Base class that represents an object in heap memory
  class Obj
  {
    private:
      // The type of the object
      Type type_;


    public:
      // Constructor
      Obj(Type type);

      // Destructor
      virtual ~Obj() = default;

      // Return the type of the object
      Type& type();
  };
}
