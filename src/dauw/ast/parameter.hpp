#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/source/location.hpp>
#include <dauw/source/token.hpp>


namespace dauw
{
  // Class that defines a parameter for a function
  class Parameter
  {
    private:
      // The name token of the parameter
      Token name_;

      // The type of the parameter
      Type type_;


    public:
      // Constructor
      Parameter(Token name, Type type);

      // Return the name of the parameter
      string_t name();

      // Return the type of the parameter
      Type type();

      // Return the location of the parameter
      Location& location();
  };
}


namespace fmt
{
  using namespace dauw;

  // Class that defines a formatter for a parameter
  template <>
  struct formatter<Parameter> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(Parameter parameter, FormatContext& ctx)
    {
      auto format = fmt::format("{}: {}", parameter.name(), parameter.type());
      return formatter<string_view_t>::format(format, ctx);
    }
  };
}
