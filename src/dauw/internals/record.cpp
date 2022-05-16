#include "record.hpp"

namespace dauw
{
  // Constructor for a record
  Record::Record(std::initializer_list<Record::container_value_type> items)
    : Obj(ObjType::RECORD)
  {
    if (items.size() > 0)
      container_ = container_type(items.begin(), items.end());
    else
      container_ = container_type();
  }

  // Iterate over the items in the container
  Record::container_type::const_iterator Record::begin()
  {
    return container_.cbegin();
  }
  Record::container_type::const_iterator Record::end()
  {
    return container_.cend();
  }

  // Return if the record contains a value with the specified name
  bool Record::contains(string_t name)
  {
    return container_.count(name) > 0;
  }

  // Return the value with the specified name in the record
  Value Record::get(string_t name)
  {
    auto it = container_.find(name);
    if (it == container_.end())
      throw std::out_of_range(fmt::format("Name {} out of range", name));

    return std::get<1>(*it);
  }

  // Put the specified value at the specified name in the record
  void Record::put(string_t name, Value value)
  {
    container_.insert(std::make_pair(name, value));
  }

  // Remove a value with the specified name from the record
  void Record::remove(string_t name)
  {
    auto it = container_.find(name);
    if (it != container_.end())
      container_.erase(it);
  }
}
