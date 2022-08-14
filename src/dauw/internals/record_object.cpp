#include "record_object.hpp"

namespace dauw
{
  // Constructor for a record
  ObjRecord::ObjRecord(std::initializer_list<ObjRecord::container_value_type> items)
    : Obj(ObjKind::RECORD, Type::type_record)
  {
    if (items.size() > 0)
      container_ = container_type(items.begin(), items.end());
    else
      container_ = container_type();
  }

  // Iterate over the items in the container
  ObjRecord::container_type::const_iterator ObjRecord::begin()
  {
    return container_.cbegin();
  }
  ObjRecord::container_type::const_iterator ObjRecord::end()
  {
    return container_.cend();
  }

  // Return if the record contains a value with the specified name
  bool ObjRecord::contains(string_t name)
  {
    return container_.count(name) > 0;
  }

  // Return the value with the specified name in the record
  Value ObjRecord::get(string_t name)
  {
    auto it = container_.find(name);
    if (it == container_.end())
      throw std::out_of_range(fmt::format("Name {} out of range", name));

    return std::get<1>(*it);
  }

  // Put the specified value at the specified name in the record
  void ObjRecord::put(string_t name, Value value)
  {
    container_.insert(std::make_pair(name, value));
  }

  // Remove a value with the specified name from the record
  void ObjRecord::remove(string_t name)
  {
    auto it = container_.find(name);
    if (it != container_.end())
      container_.erase(it);
  }
}
