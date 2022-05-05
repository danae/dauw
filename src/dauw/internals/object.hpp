#pragma once

#include <dauw/common.hpp>

#include <cstdlib>
#include <cstring>
#include <utf8.h>


namespace dauw
{
  // Enum that represents the type of an object
  enum class ObjType : char
  {
    STRING,
  };


  // Base class that represents an object in heap memory
  class Obj
  {
    private:
      // The type of the object
      ObjType type_;


    public:
      // Constructor
      Obj(ObjType type);

      // Destructor
      virtual ~Obj() = default;

      // Return the type of the object
      ObjType& type();

      // Return a representative string representation of the object
      string_t to_string();
  };


  // Class that represents a string object
  class ObjString : public Obj
  {
    private:
      // The length of the actual characters
      size_t length_;

      // The actual characters
      char* bytes_;


      // Constructor
      ObjString(size_t length, char* bytes);

      // Return the begin and end of the actual characters for iteration
      inline char* begin_() const { return bytes_; }
      inline char* end_() const { return bytes_ + length_; }


    public:
      // Typedef for the iterator
      using iterator_type = utf8::iterator<char*>;


      // Constructor that copies the specified bytes
      ObjString(const char* bytes);

      // Constructor for a string that copies another string
      ObjString(const ObjString& other);

      // Destructor
      ~ObjString();

      // Convert the string to a C-style string
      const char* c_str() const;

      // Methods for handling the runes in the string
      iterator_type rune_iterator();
      size_t rune_length();
      uint32_t rune_at(size_t pos);

      // Return if the string equals another string
      bool operator==(const ObjString& other);
      bool operator!=(const ObjString& other);

      // Concatenate another string at the end of the string
      ObjString operator+(const ObjString& other);
  };
}
