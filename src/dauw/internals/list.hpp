#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>

#include <list>


namespace dauw
{
  // Class that defines a implementation for Sequence[T] using std::list<T>
  class List : public Obj
  {
    public:
      // Type definition for the backing C++ container
      using container_type = std::list<Value>;


    private:
      // The backing C++ container of the list
      container_type container_;

      // Iterate over the items in the container
      container_type::const_iterator begin();
      container_type::const_iterator end();

      // Return an iterator at a specified index
      container_type::const_iterator pos(int index);


    public:
      // Constructor
      List(std::initializer_list<Value> items = {});

      // Return if the collection contains the specified item
      bool contains(Value item);

      // Return the number of items in the container
      int length();

      // Return if the collection contains no items
      bool empty();

      // Return the item at the specified index in the sequence
      Value at(int index);

      // Add the specified item to the collection
      void add(Value item);

      // Add all of the items in the specified collection to the collection
      void add_all(List items);

      // Remove a single instance of the specified item from the collection
      void remove(Value item);

      // Remove all of the items in the specified collection from the collection
      void remove_all(List items);

      // Remove all of the items in the collection apart from the items in the specified collection
      void retain_all(List items);

      // Removes all of the items in the collection
      void clear();

      // Insert the specified item at the specified index to the sequence
      void insert(int index, Value item);

      // Replace the specified item at the specified index in the sequence
      void replace(int index, Value item);

      // Erase the specified item at the specified index in the sequence
      void erase(int index);
  };
}
