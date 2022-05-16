#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>

#include <cstdlib>
#include <cstring>
#include <utf8.h>


namespace dauw
{
  // Class that represents a string object
  class String : public Obj
  {
    public:
      // Type definition for the iterator
      using iterator_type = utf8::iterator<char*>;


    private:
      // The length of the actual characters
      size_t length_;

      // The actual characters
      char* bytes_;


      // Constructor
      String(size_t length, char* bytes);

      // Return the begin and end of the actual characters for iteration
      inline char* begin_() const { return bytes_; }
      inline char* end_() const { return bytes_ + length_; }


    public:
      // Constructor that copies the specified bytes
      String(const char* bytes);

      // Constructor for a string that copies another string
      String(const String& other);

      // Destructor
      ~String();

      // Convert the string to a C-style string
      const char* c_str() const;

      // Methods for handling the runes in the string
      iterator_type rune_iterator();
      size_t rune_length();
      uint32_t rune_at(size_t pos);

      // Return if the string equals another string
      bool operator==(const String& other);
      bool operator!=(const String& other);

      // Concatenate another string at the end of the string
      String operator+(const String& other);
  };
}
