#include "sequence_object.hpp"

namespace dauw::internals
{
  // Constructor for a sequence
  Sequence::Sequence(std::initializer_list<Value> items)
    : Obj(ObjKind::LIST, Type::type_sequence)
  {
    if (items.size() > 0)
      container_ = container_type(items.begin(), items.end());
    else
      container_ = container_type();
  }

  // Iterate over the items in the container
  Sequence::container_type::const_iterator Sequence::begin()
  {
    return container_.cbegin();
  }
  Sequence::container_type::const_iterator Sequence::end()
  {
    return container_.cend();
  }

  // Return an iterator at a specified index
  Sequence::container_type::const_iterator Sequence::pos(int index)
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
  bool Sequence::contains(Value item)
  {
    for (auto it = container_.cbegin(); it != container_.cend(); it ++)
    {
      if (item == *it)
        return true;
    }
    return false;
  }

  // Return the number of items in the container
  int Sequence::length()
  {
    return container_.size();
  }

  // Return if the collection contains no items
  bool Sequence::empty()
  {
    return container_.empty();
  }

  // Return the item at the specified index in the sequence
  Value Sequence::at(int index)
  {
    auto it = pos(index);
    return *it;
  }

  // Add the specified item to the collection
  void Sequence::add(Value item)
  {
    container_.push_back(item);
  }

  // Add all of the items in the specified collection to the collection
  void Sequence::add_all(Sequence items)
  {
    for (auto it = items.container_.cbegin(); it != items.container_.cend(); it ++)
      add(*it);
  }

  // Remove a single instance of the specified item from the collection
  void Sequence::remove(Value item)
  {
    for (auto it = container_.cbegin(); it != container_.cend(); it ++)
    {
      if (item == *it)
        container_.erase(it);
    }
  }

  // Remove all of the items in the specified collection from the collection
  void Sequence::remove_all(Sequence items)
  {
    for (auto items_it = items.container_.cbegin(); items_it != items.container_.cend(); items_it ++)
    {
      if (contains(*items_it))
        remove(*items_it);
    }
  }

  // Remove all of the items in the collection apart from the items in the specified collection
  void Sequence::retain_all(Sequence items)
  {
    for (auto items_it = items.container_.cbegin(); items_it != items.container_.cend(); items_it ++)
    {
      if (!contains(*items_it))
        remove(*items_it);
    }
  }

  // Removes all of the items in the collection
  void Sequence::clear()
  {
    container_.clear();
  }

  // Insert the specified item at the specified index to the sequence
  void Sequence::insert(int index, Value item)
  {
    auto it = pos(index);
    container_.insert(it, item);
  }

  // Replace the specified item at the specified index in the sequence
  void Sequence::replace(int index, Value item)
  {
    auto it = pos(index);
    auto insert_it = container_.erase(it);
    container_.insert(insert_it, item);
  }

  // Erase the specified item at the specified index in the sequence
  void Sequence::erase(int index)
  {
    auto it = pos(index);
    container_.erase(it);
  }
}
