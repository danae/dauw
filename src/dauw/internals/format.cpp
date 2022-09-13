#include "format.hpp"

namespace dauw
{
  // Format a value
  string_t format(Value value, bool repr)
  {
    if (value.is_nothing())
      return format_nothing(repr);
    else if (value.is_bool())
      return format_bool(value.as_bool(), repr);
    else if (value.is_int())
      return format_int(value.as_int(), repr);
    else if (value.is_rune())
      return format_rune(value.as_rune(), repr);
    else if (value.is_float())
      return format_float(value.as_float(), repr);
    else if (value.is_obj())
      return format_obj(value.as_obj(), repr);
    else
      throw std::out_of_range("Invalid value");
  }

  // Format a nothing value
  string_t format_nothing(bool repr)
  {
    return "nothing";
  }

  // Format a bool value
  string_t format_bool(dauw_bool_t value, bool repr)
  {
    return fmt::format("{}", value ? "true" : "false");
  }

  // Format an int value
  string_t format_int(dauw_int_t value, bool repr)
  {
    return fmt::format("{}", value);
  }

  // Format a rune value
  string_t format_rune(dauw_rune_t value, bool repr)
  {
    return fmt::format("{}", dauw::utils::rune_pack_to_str(value));
  }

  // Format a float value
  string_t format_float(dauw_float_t value, bool repr)
  {
    if (std::isinf(value))
      return std::signbit(value) ? "-infinity" : "infinity";
    else if (std::isnan(value))
      return "nan";
    else
      return fmt::format("{:#}", value);
  }

  // Format an object
  string_t format_obj(Obj* obj, bool repr)
  {
    //if (obj->type() == Type::type_record)
    //  return format_obj_record(static_cast<ObjRecord*>(obj), repr);
    //else if (obj->type() == Type::type_sequence)
    //  return format_obj_sequence(static_cast<ObjSequence*>(obj), repr);
    if (obj->type() == Type::type_string)
      return format_obj_string((ObjString*)obj, repr);
    else
      return fmt::format("<object {} at {:#014x}>", obj->type(), (uintptr_t)(obj));
  }

  // Format a record object
  string_t format_obj_record(ObjRecord* obj, bool repr)
  {
    return "{}";
  }

  // Format a sequence object
  string_t format_obj_sequence(ObjSequence* obj, bool repr)
  {
    return "[]";
  }

  // Format a string object
  string_t format_obj_string(ObjString* obj, bool repr)
  {
    //string_t u;
    //for (auto it = obj->begin(); it != obj->end(); it++)
    //  utf8::append((char32_t)*it, u);

    return fmt::format("{}", obj->c_str());
  }
}
