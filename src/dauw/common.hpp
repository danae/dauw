#pragma once

#include <cmath>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/format.h>

#include <utf8.h>


// Defines for versions
#ifndef DAUW_VERSION
  #define DAUW_VERSION "0.1.0"
#endif

#ifndef DAUW_GIT_BRANCH
  #define DAUW_GIT_BRANCH "unknown"
#endif

#ifndef DAUW_GIT_COMMIT_HASH
  #define DAUW_GIT_COMMIT_HASH "unknown"
#endif

// Defines for exit codes
#define DAUW_EXIT_OK 0
#define DAUW_EXIT_USAGE 64
#define DAUW_EXIT_DATAERR 65
#define DAUW_EXIT_SOFTWAREERR 70
#define DAUW_EXIT_OSERR 71
#define DAUW_EXIT_IOERR 74
#define DAUW_EXIT_CONFIG 78


namespace dauw
{
  // Type definitions for common types
  using string_t = std::string;
  using string_view_t = std::string_view;

  // Type definitions for internal values
  using dauw_bool_t = bool;
  using dauw_int_t = int64_t;
  using dauw_float_t = double;
  using dauw_rune_t = uint32_t;


  // Class that defines a base for all exceptions and errors
  class Exception
  {
    private:
      // The message of the exception
      string_t message_;

      // The previous exception that caused the exception to happen
      Exception* previous_;


    public:
      // Constructor
      inline Exception(string_t message, Exception* previous) : message_(message), previous_(previous) {}
      inline Exception(string_t message) : Exception(message, nullptr) {}

      // Return the message of the exception
      inline string_t message() { return message_; }

      // Return the previous exception that caused the exception to happen
      inline Exception* previous() { return previous_; }
  };
}
