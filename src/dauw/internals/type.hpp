#pragma once

#include <dauw/common.hpp>


namespace dauw
{
  // Enum that represents the kind of a type
  enum class TypeKind : uint8_t
  {
    NOTHING,
    BOOL,
    INT,
    REAL,
    RUNE,
    STRING,
    SEQUENCE,
    RECORD,
    FUNCTION,
    TYPE,
  };


  // Class that represents a type
  class Type
  {
    private:
      // The kind of the type
      TypeKind kind_;

      // The canonical name of the type
      string_t name_;

      // The inner types of the type
      std::vector<Type> inners_;


    public:
      // Constructor
      Type(TypeKind kind, string_t name, std::initializer_list<Type> inners = {});

      // Return the kind of the type
      TypeKind kind();

      // Return the canonical name of the type
      string_t name();

      // Return the number of inner types of the type
      size_t inner_count();

      // Return the inner type of the type at the specified index
      Type inner(size_t index);

      // Return if the type equals another type
      bool operator==(const Type& other);
      bool operator!=(const Type& other);


      // Definitions for global types
      static Type type_nothing;
      static Type type_bool;
      static Type type_int;
      static Type type_real;
      static Type type_rune;
      static Type type_string;
      static Type type_sequence;
      static Type type_record;
      static Type type_function;
      static Type type_type;
  };
}


namespace fmt
{
  using namespace dauw;
  using namespace dauw::internals;

  // Class that defines a formatter for a type
  template <>
  struct formatter<Type> : formatter<string_view_t>
  {
    inline string_t stringify(Type type)
    {
      string_t format = type.name();
      if (type.inner_count() > 0)
      {
        format += "[";
        for (auto i = 0; i < type.inner_count(); i ++)
        {
          if (i > 0)
            format += ", ";
          format += fmt::format("{}", type.inner(i));
        }
        format += "]";
      }
      return format;
    }

    template <typename FormatContext>
    auto format(Type type, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(stringify(type), ctx);
    }
  };
}
