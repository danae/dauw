#include "type.hpp"

namespace dauw
{
  // Initialize the definitions for global types
  Type Type::type_nothing = Type(TypeKind::NOTHING, "Nothing");
  Type Type::type_bool = Type(TypeKind::BOOL, "Bool");
  Type Type::type_int = Type(TypeKind::INT, "Int");
  Type Type::type_float = Type(TypeKind::FLOAT, "Real");
  Type Type::type_string = Type(TypeKind::STRING, "String");
  Type Type::type_sequence = Type(TypeKind::SEQUENCE, "Sequence");
  Type Type::type_record = Type(TypeKind::RECORD, "Record");
  Type Type::type_function = Type(TypeKind::FUNCTION, "Function");
  Type Type::type_type = Type(TypeKind::TYPE, "Type");


  // Constructor for a type
  Type::Type(TypeKind kind, string_t name, std::initializer_list<Type> inners)
    : kind_(kind), name_(name), inners_(std::vector<Type>(inners.begin(), inners.end()))
  {
  }

  // Return the kind of the type
  TypeKind Type::kind()
  {
    return kind_;
  }

  // Return the canonical name of the type
  string_t Type::name()
  {
    return name_;
  }

  // Return the number of inner types of the type
  size_t Type::inner_count()
  {
    return inners_.size();
  }

  // Return the inner type of the type at the specified index
  Type Type::inner(size_t index)
  {
    return inners_[index];
  }

  // Return if the type equals another type
  bool Type::operator==(const Type& other)
  {
    // TODO: Add checking of inner types
    return kind_ == other.kind_ && name_ == other.name_;
  }
  bool Type::operator!=(const Type& other)
  {
    return !(*this == other);
  }
}
