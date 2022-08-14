#include "sequence_object.hpp"

namespace dauw
{
  // Constructor for a sequence
  ObjSequence::ObjSequence(std::initializer_list<Value> items)
    : Obj(ObjKind::LIST, Type::type_sequence)
  {
    if (items.size() > 0)
      container_ = container_type(items.begin(), items.end());
    else
      container_ = container_type();
  }

  // Iterate over the items in the container
  ObjSequence::container_type::const_iterator ObjSequence::begin()
  {
    return container_.cbegin();
  }
  ObjSequence::container_type::const_iterator ObjSequence::end()
  {
    return container_.cend();
  }

  // Return an iterator at a specified index
  ObjSequence::container_type::const_iterator ObjSequence::pos(int index)
  {
    if (index >= 0 && index < container_.size())
    {
      auto it = container_.cbegin();
      for (auto i = 0; i < index;)
        it ++;
      return it;
    }
    else if (index < 0 && index <= -container_.size())
    {
      auto it = container_.cbegin();
      for (auto i = 0; i < container_.size() - index;)
        it ++;
      return it;
    }
    else
      throw std::out_of_range(fmt::format("Index {} is out of range", index));
  }

  // Return if the collection contains the specified item
  bool ObjSequence::contains(Value item)
  {
    for (auto it = container_.cbegin(); it != container_.cend(); it ++)
    {
      if (item == *it)
        return true;
    }
    return false;
  }

  // Return the number of items in the container
  int ObjSequence::length()
  {
    return container_.size();
  }

  // Return if the collection contains no items
  bool ObjSequence::empty()
  {
    return container_.empty();
  }

  // Return the item at the specified index in the sequence
  Value ObjSequence::at(int index)
  {
    auto it = pos(index);
    return *it;
  }

  // Add the specified item to the collection
  void ObjSequence::add(Value item)
  {
    container_.push_back(item);
  }

  // Add all of the items in the specified collection to the collection
  void ObjSequence::add_all(ObjSequence items)
  {
    for (auto it = items.container_.cbegin(); it != items.container_.cend(); it ++)
      add(*it);
  }

  // Remove a single instance of the specified item from the collection
  void ObjSequence::remove(Value item)
  {
    for (auto it = container_.cbegin(); it != container_.cend(); it ++)
    {
      if (item == *it)
        container_.erase(it);
    }
  }

  // Remove all of the items in the specified collection from the collection
  void ObjSequence::remove_all(ObjSequence items)
  {
    for (auto items_it = items.container_.cbegin(); items_it != items.container_.cend(); items_it ++)
    {
      if (contains(*items_it))
        remove(*items_it);
    }
  }

  // Remove all of the items in the collection apart from the items in the specified collection
  void ObjSequence::retain_all(ObjSequence items)
  {
    for (auto items_it = items.container_.cbegin(); items_it != items.container_.cend(); items_it ++)
    {
      if (!contains(*items_it))
        remove(*items_it);
    }
  }

  // Removes all of the items in the collection
  void ObjSequence::clear()
  {
    container_.clear();
  }

  // Insert the specified item at the specified index to the sequence
  void ObjSequence::insert(int index, Value item)
  {
    auto it = pos(index);
    container_.insert(it, item);
  }

  // Replace the specified item at the specified index in the sequence
  void ObjSequence::replace(int index, Value item)
  {
    auto it = pos(index);
    auto insert_it = container_.erase(it);
    container_.insert(insert_it, item);
  }

  // Erase the specified item at the specified index in the sequence
  void ObjSequence::erase(int index)
  {
    auto it = pos(index);
    container_.erase(it);
  }
}
