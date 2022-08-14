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

      // Return a string representation of the object
      virtual string_t str();
  };
}


namespace fmt
{
  using namespace dauw;

  // Class that defines a formatter for an object
  template <>
  struct formatter<Obj> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(Obj object, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(object.str(), ctx);
    }
  };
}
