#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>

#include <unordered_map>


namespace dauw
{
  // Forward declarations
  class Type;

  // Type definitions for pointers
  using type_ptr = std::shared_ptr<Type>;
  using type_ptr_list = std::vector<type_ptr>;
  using type_ptr_initializer_list = std::initializer_list<type_ptr>;


  // Enum that represents the kind of a type
  enum class TypeKind : uint8_t
  {
    UNDEFINED,
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
    MAYBE_TYPE,
    INTERSECTION_TYPE,
    UNION_TYPE,
  };


  // Class that represents a type
  class Type : public Obj, public std::enable_shared_from_this<Type>
  {
    private:
      // Return the name for the specified type kind
      static string_t type_kind_name_(TypeKind kind);


      // The kind of the type
      TypeKind kind_;

      // The anonical name of the type
      string_t name_;

      // The inner types of the type
      type_ptr_list inner_types_;


    public:
      // Constructor
      Type(TypeKind kind, string_t name, type_ptr_initializer_list inner_types = {});
      Type(TypeKind kind, type_ptr_initializer_list inner_types = {});

      // Return the kind of the type
      TypeKind kind();

      // Return the anonical name of the type
      string_t name();

      // Return the number of inner types of the type
      size_t inner_type_count();

      // Return the inner type of the type at the specified index
      type_ptr inner_type(size_t index);
  };
}
