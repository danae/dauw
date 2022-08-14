#pragma once

#include <dauw/common.hpp>
#include <dauw/internals/object.hpp>
#include <dauw/internals/value.hpp>

#include <cstdlib>
#include <cstring>
#include <utf8.h>


namespace dauw
{
  class StringException;


  // Class that represents a string
  class String : public Obj
  {
    public:
      // Type definitions
      using value_type = dauw_rune_t;
      using iterator_type = utf8::iterator<char*>;


    private:
      // The length of the actual characters of the string
      size_t length_;

      // The actual characters of the string
      char* bytes_;


    public:
      // Default constructor
      String();

      // Constructor from another source
      String(const char* bytes);
      String(const String& other);
      template <typename C> inline String(C c) : String(to_bytes(c)) {}

      String& operator=(const char* bytes);
      String& operator=(const String& other);
      template <typename C> inline String& operator=(C c) { return this->operator=(to_bytes(c)); }

      // Destructor
      ~String();

      // Append to the string from another source
      String& append(const char* bytes);
      String& append(const String& other);
      template <typename C> inline String& append(C c) { return append(to_bytes(c)); }

      inline String& operator+=(const char* bytes) { return append(bytes); }
      inline String& operator+=(const String& other) { return append(other); }
      template <typename C> inline String& operator+=(C c) { return append(c); }


      // Return the actual characters of the string
      const char* c_str();

      // Return the length in code points of the string
      size_t length();

      // Return the code point at the specified position of the string
      value_type at(size_t pos);

      // Return an iterator over the code points of the string
      iterator_type begin();
      iterator_type end();

      // Compare the string to another string
      int compare(String& other);

      inline bool operator==(String& other) { return compare(other) == 0; }
      inline bool operator!=(String& other) { return compare(other) != 0; }
      inline bool operator<(String& other) { return compare(other) < 0; }
      inline bool operator<=(String& other) { return compare(other) <= 0; }
      inline bool operator>(String& other) { return compare(other) > 0; }
      inline bool operator>=(String& other) { return compare(other) >= 0; }

      // Return a string representation of the string
      virtual string_t str() override;


      // Convert an iterable of code points to a C-string
      template <typename C>
      static const char* to_bytes(C c)
      {
        try
        {
          std::vector<char> bytes_vector;
          utf8::utf32to8(std::begin(c), std::end(c), std::back_inserter(bytes_vector));

          char bytes[bytes_vector.size()];
          std::copy(bytes_vector.begin(), bytes_vector.end(), bytes);
          return (const char*)bytes;
        }
        catch (utf8::invalid_code_point& ex)
        {
          throw StringException("The provided array of runes is not a valid UTF-32 encoded string");
        }
      }
  };


  // Exception thrown when processing a string fails
  class StringException : public Exception
  {
    public:
      inline StringException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline StringException(string_t message) : Exception(message, nullptr) {}
  };
}
