#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>

#include <unordered_map>


namespace dauw
{
  // Class that defines a implementation for Record using std::unordered_map<T>
  class ObjRecord : public Obj
  {
    public:
      // Type definition for the backing C++ container
      using container_type = std::unordered_map<string_t, Value>;
      using container_value_type = container_type::value_type;


    private:
      // The backing C++ container of the list
      container_type container_;

      // Iterate over the items in the container
      container_type::const_iterator begin();
      container_type::const_iterator end();


    public:
      // Constructor
      ObjRecord(std::initializer_list<container_value_type> items = {});

      // Return if the record contains a value with the specified name
      bool contains(string_t name);

      // Return the value with the specified name in the record
      Value get(string_t name);

      // Put the specified value at the specified name in the record
      void put(string_t name, Value value);

      // Remove a value with the specified name from the record
      void remove(string_t name);
  };
}
