#include "globals.hpp"

namespace dauw
{
  // Create and populate a record of globals
  Record globals()
  {
    // Create the globals record
    Record g;

    // Register the global types
    auto type_type = std::make_shared<Type>(TypeKind::TYPE);

    g.put("Nothing", Value::of_obj(std::make_shared<Type>(TypeKind::NOTHING).get(), type_type));
    g.put("Bool", Value::of_obj(std::make_shared<Type>(TypeKind::BOOL).get(), type_type));
    g.put("Int", Value::of_obj(std::make_shared<Type>(TypeKind::INT).get(), type_type));
    g.put("Real", Value::of_obj(std::make_shared<Type>(TypeKind::REAL).get(), type_type));
    g.put("Rune", Value::of_obj(std::make_shared<Type>(TypeKind::RUNE).get(), type_type));
    g.put("String", Value::of_obj(std::make_shared<Type>(TypeKind::STRING).get(), type_type));
    g.put("Sequence", Value::of_obj(std::make_shared<Type>(TypeKind::SEQUENCE).get(), type_type));
    g.put("Record", Value::of_obj(std::make_shared<Type>(TypeKind::RECORD).get(), type_type));
    g.put("Function", Value::of_obj(std::make_shared<Type>(TypeKind::FUNCTION).get(), type_type));
    g.put("Type", Value::of_obj(std::make_shared<Type>(TypeKind::TYPE).get(), type_type));

    // Return the globals record
    return g;
  }
}
