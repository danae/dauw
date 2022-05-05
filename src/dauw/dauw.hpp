#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/interpreter.hpp>
#include <dauw/source/lexer.hpp>
#include <dauw/source/location.hpp>
#include <dauw/source/parser.hpp>
#include <dauw/vm/vm.hpp>

#include <filesystem>
#include <fstream>

#include <linenoise.hpp>


namespace dauw
{
  // Class that defines the high-level code of the application
  class Dauw : public std::enable_shared_from_this<Dauw>
  {
    private:
      // The virtual machine for the application
      std::shared_ptr<VM> vm_;

      // The interpreter for the application
      std::shared_ptr<Interpreter> interpreter_;

      // The current source string
      string_t current_source_;
      string_t current_source_name_;


    public:
      // Constructor
      Dauw();

      // Run code from a source string
      int run(string_t source, string_t source_name = "<prompt>");

      // Run code from a read-eval-print loop
      int run_repl();

      // Run code from a file
      int run_file(string_t file);

      // Report an error
      void report(string_t message);

      // Report an error with a location
      void report(Location location, string_t message);

      // Report an error exception
      void report_error(Error& error, string_t error_type = "Error");
  };
}
