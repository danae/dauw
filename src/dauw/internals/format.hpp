#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/record_object.hpp>
#include <dauw/internals/sequence_object.hpp>
#include <dauw/internals/string_object.hpp>
#include <dauw/internals/type.hpp>
#include <dauw/internals/value.hpp>


namespace dauw
{
  // Format a value
  string_t format(Value value, bool repr = false);

  // Format a nothing value
  string_t format_nothing(bool repr = false);

  // Format a bool value
  string_t format_bool(dauw_bool_t value, bool repr = false);

  // Format an int value
  string_t format_int(dauw_int_t value, bool repr = false);

  // Format a float value
  string_t format_float(dauw_float_t value, bool repr = false);

  // Format an object
  string_t format_obj(Obj* obj, bool repr = false);

  // Format a record object
  string_t format_obj_record(ObjRecord* obj, bool repr = false);

  // Format a sequence object
  string_t format_obj_sequence(ObjSequence* obj, bool repr = false);

  // Format a string object
  string_t format_obj_string(ObjString* obj, bool repr = false);
}


namespace fmt
{
  using namespace dauw;

  // Class that defines a formatter for a value
  template <>
  struct formatter<Value> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(Value value, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(dauw::format(value), ctx);
    }
  };

  // Class that defines a formatter for an object
  template <>
  struct formatter<Obj> : formatter<string_view_t>
  {
    template <typename FormatContext>
    auto format(Obj object, FormatContext& ctx)
    {
      return formatter<string_view_t>::format(dauw::format_obj(&object), ctx);
    }
  };
}
