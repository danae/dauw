#pragma once

#include "common.h"
#include "errors.h"
#include "interpreter.h"
#include "source_lexer.h"
#include "source_location.h"
#include "source_parser.h"
#include "vm.h"

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
