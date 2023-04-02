#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/code/lexer.hpp>
#include <dauw/code/location.hpp>
#include <dauw/code/parser.hpp>
#include <dauw/code/source.hpp>
#include <dauw/interpreter/interpreter.hpp>
#include <dauw/interpreter/type_resolver.hpp>

#include <filesystem>
#include <fstream>

#include <linenoise.hpp>


namespace dauw
{
  // Class that defines the high-level code of the application
  class Dauw
  {
    public:
      // Constructor
      Dauw();

      // Run code from a source file
      int run(source_ptr source);

      // Run code from a read-eval-print loop
      int run_repl();

      // Run code from a file
      int run_file(string_t file);
  };
}
