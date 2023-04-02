#pragma once

#include <dauw/common.hpp>
#include <dauw/code/location.hpp>
#include <dauw/utils/regex.hpp>
#include <dauw/utils/string.hpp>

#include <filesystem>
#include <fstream>


namespace dauw
{
  // Forward declarations
  class Source;


  // Type definitions for pointers
  using source_ptr = std::shared_ptr<Source>;


  // Class that defines a source file
  class Source
  {
    private:
      // Initialize the regex patterns for the source
      static utils::Regex line_pattern_;


      // The file of the source
      string_t file_;

      // The source string of the source
      string_t source_;

      // The source string split by line of the source
      std::vector<string_t> source_lines_;


    public:
      // Create a shared pointer to a source
      static source_ptr create(string_t file, string_t source);

      // Create a shared pointer to a source read from a file
      static source_ptr create_read(string_t file);


      // Constructor
      Source(string_t file, string_t source);

      // Return the file of the source
      string_t& file();

      // Return the source string of the source
      string_t& source();

      // Return The source string split by line of the source
      std::vector<string_t> source_lines();

      // Format a location in the source
      string_t format(Location location);
  };


  // Exception thrown when processing a source file fails
  class SourceException : public Exception
  {
    public:
      // Constructor
      inline SourceException(string_t message, Exception* previous) : Exception(message, previous) {}
      inline SourceException(string_t message) : Exception(message, nullptr) {}
  };
}
