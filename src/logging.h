#pragma once

#include "common.h"
#include "errors.h"
#include "token.h"


namespace dauw
{
  // Enum that defines the logging level for a log message
  enum class LoggerLevel : size_t
  {
    ERROR,
    WARNING,
    INFO,
    VERBOSE,
  };


  // Class that defines a logger
  class Logger
  {
    private:
      // The current logging level
      LoggerLevel level_;


    public:
      // Constructor
      Logger(LoggerLevel level);

      // Log a message
      template <typename... T> void log(LoggerLevel level, string_t message, T... args)
      {
        if (level == LoggerLevel::ERROR && level_ >= LoggerLevel::ERROR)
          fmt::print(fmt::fg(fmt::color::crimson), message + "\n", args...);
        else if (level == LoggerLevel::WARNING && level_ >= LoggerLevel::WARNING)
          fmt::print(fmt::fg(fmt::color::gold), message + "\n", args...);
        else if (level == LoggerLevel::INFO && level_ >= LoggerLevel::INFO)
          fmt::print(message + "\n", args...);
        else if (level == LoggerLevel::VERBOSE && level_ >= LoggerLevel::VERBOSE)
          fmt::print(message + "\n", args...);
      }

      // Log an error message
      template <typename... T> void error(string_t message, T... args)
      {
        log(LoggerLevel::ERROR, message, args...);
      }

      // Log a warning message
      template <typename... T> void warning(string_t message, T... args)
      {
        log(LoggerLevel::WARNING, message, args...);
      }

      // Log an informational message
      template <typename... T> void info(string_t message, T... args)
      {
        log(LoggerLevel::INFO, message, args...);
      }

      // Log an informational message
      template <typename... T> void verbose(string_t message, T... args)
      {
        log(LoggerLevel::VERBOSE, message, args...);
      }
  };


  // Log an error
  void log_group(string_t message);
  void log_group_end();

  void log_verbose(string_t message);
  void log_verbose(const fmt::text_style& text_style, string_t message);

  void log_error(string_t message, Location location, string_t source);
  void log_error(Error error, string_t source);
}
