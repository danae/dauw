#pragma once

#include <dauw/common.hpp>


namespace dauw
{
  // Class that represents a type
  class Type
  {
    private:
      // The name of the type
      string_t name_;

      // The base type of the type
      const Type* base_;


    public:
      // Constructor
      Type(string_t name, const Type* base_ = nullptr);

      // Return the name of the type
      string_t& name();

      // Return the base type of the type
      const Type* base();

      // Return if the type equals another type
      bool operator==(const Type& other);
      bool operator!=(const Type& other);
  };


  // Definitions of primary types
  const Type type_any = Type("Any");

  const Type type_any_val = Type("AnyVal", &type_any);
  const Type type_nothing = Type("Nothing", &type_any_val);
  const Type type_bool = Type("Bool", &type_any_val);
  const Type type_int = Type("Int", &type_any_val);
  const Type type_rune = Type("Rune", &type_any_val);
  const Type type_real = Type("Real", &type_any_val);

  const Type type_any_ref = Type("AnyRef", &type_any);
  const Type type_string = Type("String", &type_any_ref);
}


namespace fmt
{
  using namespace dauw;

  // Class that defines a formatter for a type
  template <>
  struct formatter<Type> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(Type type, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(type.name(), ctx);
    }
  };
}
