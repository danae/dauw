#pragma once

#include <dauw/common.hpp>
#include <dauw/errors.hpp>
#include <dauw/backend/code.hpp>
#include <dauw/backend/compiler.hpp>
#include <dauw/backend/type_resolver.hpp>
#include <dauw/backend/vm.hpp>
#include <dauw/frontend/lexer.hpp>
#include <dauw/frontend/location.hpp>
#include <dauw/frontend/parser.hpp>
#include <dauw/frontend/source.hpp>

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
      std::shared_ptr<backend::VM> vm_;


    public:
      // Constructor
      Dauw();

      // Run code from a source file
      int run(frontend::Source* source);

      // Run code from a read-eval-print loop
      int run_repl();

      // Run code from a file
      int run_file(string_t file);
  };
}
