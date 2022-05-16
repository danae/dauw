#include "type.hpp"

namespace dauw
{
  // Return the name for the specified type kind
  string_t Type::type_kind_name_(TypeKind kind)
  {
    switch (kind)
    {
      case TypeKind::NOTHING: return "Nothing";
      case TypeKind::BOOL: return "Bool";
      case TypeKind::INT: return "Int";
      case TypeKind::REAL: return "Real";
      case TypeKind::RUNE: return "Rune";
      case TypeKind::STRING: return "String";
      case TypeKind::SEQUENCE: return"Sequence";
      case TypeKind::RECORD: return"Record";
      case TypeKind::FUNCTION: return "Function";
      case TypeKind::TYPE: return "Type";
      default: throw std::out_of_range("Cannot infer type name");
    }
  }


  // Constructor for a type
  Type::Type(TypeKind kind, string_t name, type_ptr_initializer_list inner_types)
    : Obj(ObjType::TYPE), kind_(kind), name_(name), inner_types_(type_ptr_list(inner_types.begin(), inner_types.end()))
  {
  }

  // Constructor for a type where the name will be inferred
  Type::Type(TypeKind kind, type_ptr_initializer_list inner_types)
    : Type(kind, type_kind_name_(kind), inner_types)
  {
  }

  // Return the kind of the type
  TypeKind Type::kind()
  {
    return kind_;
  }

  // Return the anonical name of the type
  string_t Type::name()
  {
    return name_;
  }

  // Return the number of inner types of the type
  size_t Type::inner_type_count()
  {
    return inner_types_.size();
  }

  // Return the inner type of the type at the specified index
  type_ptr Type::inner_type(size_t index)
  {
    return inner_types_[index];
  }
}
