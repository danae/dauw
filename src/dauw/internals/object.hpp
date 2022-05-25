#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/type.hpp>


namespace dauw::internals
{
  // Enum that represents the kind of an object
  enum class ObjKind : char
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
      // The object kind of the object
      ObjKind kind_;

      // The type of the object
      Type type_;


    public:
      // Constructor
      Obj(ObjKind kind, Type type);

      // Destructor
      virtual ~Obj() = default;

      // Return the object kind of the object
      ObjKind& kind();

      // Return the type of the object
      Type& type();
  };
}


namespace fmt
{
  using namespace dauw;
  using namespace dauw::internals;

  // Class that defines a formatter for an object
  template <>
  struct formatter<Obj> : formatter<string_view_t>
  {
    inline string_t stringify(Obj object)
    {
      return fmt::format("<object {} at {:#012x}>", object.type(), (uintptr_t)(&object));
    }

    template <typename FormatContext>
    auto format(Obj object, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(object), ctx);
    }
  };
}
