#pragma once

#include <dauw/common.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/utils/string.hpp>

#include <filesystem>
#include <fstream>


namespace dauw::frontend
{
  // Class that defines a source file
  class Source
  {
    private:
      // The file the source is contained in
      string_t file_;

      // The actual source
      string_t source_;


    public:
      // Constructor
      Source(string_t file, string_t source);

      // Return the file the source is contained in
      string_t& file();

      // Return the actual source
      string_t& source();

      // Format a location in the source
      string_t format(Location location);


      // Read a source from a file in the file system
      static Source* read(string_t file);
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
