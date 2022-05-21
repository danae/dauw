#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/ast/analyzer.hpp>
#include <dauw/ast/compiler.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/backend/vm.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/parser.hpp>

#include <filesystem>
#include <fstream>

#include <linenoise.hpp>


namespace dauw
{
  // Class that defines the high-level code of the application
  class Dauw : public std::enable_shared_from_this<Dauw>
  {
    private:
      // The error reporter for the application
      std::shared_ptr<ErrorReporter> reporter_;

      // The virtual machine for the application
      std::shared_ptr<VM> vm_;

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
  };
}
